#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <NativeUI/Widgets.h>
#include <NativeUI/WidgetUtil.h>

using namespace MAUtil;
using namespace NativeUI;

/**
 * This is main UI component that holds other components.
 * The idea is to have the root UI separate, so that
 * other UI components can be added in a modular way.
 */
class MainUI
{
public:
	MainUI()
	{
		createUI();
	}

	void createUI()
	{
		// Create a NativeUI screen that will hold layout and widgets.
		mScreen = new Screen();

		// Create a RelativeLayout to use as the root layout.
		mLayout = new RelativeLayout();

		// Add the layout to the screen.
		mScreen->setMainWidget(mLayout);
	}

	void addChild(Widget* widget)
	{
		mLayout->addChild(widget);
	}

	void removeChild(Widget* widget)
	{
		mLayout->removeChild(widget);
	}

	void show()
	{
		mScreen->show();
	}

private:
    Screen* mScreen;
    RelativeLayout* mLayout;
};

/**
 * The UI module that shows the live camera preview.
 * Has a button for taking a snapshot.
 */
class PreviewUI
{
public:
	PreviewUI()
	{
		createUI();
	}

	void createUI()
	{
		// Create a Vertical Layout that will hold widgets.
		mLayout = new VerticalLayout();
		/*
		mLayout->setTopPosition(0);
		mLayout->setLeftPosition(0);
		mLayout->setHeight(500);
		mLayout->setWidth(500);
		*/
		mLayout->fillSpaceHorizontally();
		mLayout->wrapContentVertically();

		// Create Camera Preview.
		mCameraPreview = new CameraPreview();
		mCameraPreview->fillSpaceHorizontally();
		mCameraPreview->fillSpaceVertically();
		mLayout->addChild(mCameraPreview);

		// Create snapshot button.
		mButton = new Button();
		mButton->fillSpaceHorizontally();
		mButton->wrapContentVertically();
		mLayout->addChild(mButton);
		updateSnapshotButtonText("show");
	}

	void updateSnapshotButtonText(String state)
	{
		if (state == "show")
		{
			mButton->setText("Take Snapshot");
		}
		else
		{
			mButton->setText("Hide Snapshot");
		}
	}

	void addButtonListener(ButtonListener* listener)
	{
		mButton->addButtonListener(listener);
	}

	void bindToCurrentCamera()
	{
		mCameraPreview->bindToCurrentCamera();
	}

	Widget* getTopWidget()
	{
		return mLayout;
	}

private:
    VerticalLayout* mLayout;
    Button* mButton;
    CameraPreview* mCameraPreview;
};

/**
 * TODO: Comment.
 */
class SnapshotUI
{
public:
	SnapshotUI()
	{
		createUI();
	}

	void createUI()
	{
		// Initiali<e image object handles.
		mImage = NULL;
		mImageHandle = NULL;

		// Create the image widget used to display snapshots
		// (it will be added to the layout dynamically).
		mImageWidget = new Image();
	}

	Widget* getTopWidget()
	{
		return mImageWidget;
	}

	void obtainImage()
	{
		if (!mImage)
		{
			// Grab image from camera.
			mImage = maCreatePlaceholder();

			// Choose the smallest camera snapshot format.
			int cameraFormat = maCameraFormatNumber() - 1;
			int snapshotResult = maCameraSnapshot(cameraFormat, mImage);
			if (snapshotResult < 0)
			{
				// An error occurred while taking the snapshot.
				releaseImage();
				return;
			}

			// Create a bitmap the image widget can display.
			mImageHandle = maCreatePlaceholder();
			int result = maCreateImageFromData(
				mImageHandle,
				mImage,
				0,
				maGetDataSize(mImage));
			if (result != RES_OK)
			{
				releaseImage();
				return;
			}

			// Position and size image widget.
			MAExtent screenSize = maGetScrSize();
			mImageWidget->setTopPosition(50);
			mImageWidget->setLeftPosition(50);
			mImageWidget->setWidth(EXTENT_X(screenSize) - 100);
			mImageWidget->setHeight(EXTENT_Y(screenSize) - 150);
			mImageWidget->setBackgroundColor(0x770000);
			mImageWidget->setAlpha(1.0);

			// Show the image in the image widget.
			mImageWidget->setImage(mImageHandle);
			//mImageWidget->setScaleMode(IMAGE_SCALE_XY);
			mImageWidget->setScaleMode(IMAGE_SCALE_PRESERVE_ASPECT);
		}
	}

	void releaseImage()
	{
		if (mImage)
		{
			// Deallocate the image.
			maDestroyPlaceholder(mImage);
			mImage = NULL;
		}

		if (mImageHandle)
		{
			// Deallocate the image.
			maDestroyPlaceholder(mImageHandle);
			mImageHandle = NULL;
		}
	}

	bool imageIsVisible()
	{
		return mImage;
	}

	bool imageIsHidden()
	{
		return !mImage;
	}

private:
    Image* mImageWidget;
    MAHandle mImage;
    MAHandle mImageHandle;
};

/**
 * Simple test application for the Camera API.
 * Displays a camera preview widget and a button
 * that currently has no functionality.
 */
class SimpleCameraMoblet :
	public Moblet,
	public FocusListener,
	public ButtonListener
{
public:
	/**
	 * The constructor creates the user interface.
	 */
	SimpleCameraMoblet()
	{
		setScreenOrientation();
		createFocusListener();
		createUI();
		startCameraPreview();
	}

	void setScreenOrientation()
	{
		// Android and Windows Phone.
		maScreenSetOrientation(SCREEN_ORIENTATION_DYNAMIC);

		// iOS and Windows Phone.
		maScreenSetSupportedOrientations(
			MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT |
			MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT |
			MA_SCREEN_ORIENTATION_PORTRAIT |
			MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN);
	}

	void createFocusListener()
	{
		addFocusListener(this);
	}

	void focusLost()
	{
		maCameraStop();
	}

	void focusGained()
	{
		maCameraStart();
	}

	void createUI()
	{
		// Create UI components.
		mMainUI = new MainUI();
		mPreviewUI = new PreviewUI();
		mSnapshotUI = new SnapshotUI();

		// Configure components.
		mMainUI->addChild(mPreviewUI->getTopWidget());
		mPreviewUI->addButtonListener(this);

		mMainUI->show();
	}

	void startCameraPreview()
	{
		maCameraSelect(0);
		mPreviewUI->bindToCurrentCamera();
		maCameraStart();
	}

	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			maCameraStop();
			close();
		}
	}

	void buttonClicked(Widget* button)
	{
		if (mSnapshotUI->imageIsHidden())
		{
			// Show the camera snapshot.
			mSnapshotUI->obtainImage();
			maCameraStart(); // Hack to start the camera after snap.
			mMainUI->addChild(mSnapshotUI->getTopWidget());
			mPreviewUI->updateSnapshotButtonText("hide");
		}
		else
		{
			// Hide the camera snapshot.
			mSnapshotUI->releaseImage();
			mMainUI->removeChild(mSnapshotUI->getTopWidget());
			mPreviewUI->updateSnapshotButtonText("show");
		}
	}

private:
    MainUI* mMainUI;
    PreviewUI* mPreviewUI;
    SnapshotUI* mSnapshotUI;
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new SimpleCameraMoblet());
	return 0;
}
