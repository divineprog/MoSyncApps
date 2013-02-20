#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <NativeUI/Widgets.h>
#include <NativeUI/WidgetUtil.h>

using namespace MAUtil;
using namespace NativeUI;

/**
 * TODO: Comment.
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
		mLayout->addChild(widget);
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
 * TODO: Comment.
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
		mLayout->setTopPosition(0);
		mLayout->setLeftPosition(0);
		mLayout->setHeight(500);
		mLayout->setWidth(500);
		//mLayout->fillSpaceHorizontally();
		//mLayout->wrapContentVertically();

		// Create Camera Preview.
		mCameraPreview = new CameraPreview();
		mCameraPreview->fillSpaceHorizontally();
		mCameraPreview->fillSpaceVertically();
		mLayout->addChild(mCameraPreview);

		// Create a button.
		mButton = new Button();
		mButton->setText("Take Snapshot");
		mButton->fillSpaceHorizontally();
		mButton->wrapContentVertically();
		mLayout->addChild(mButton);
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
		// Create the image widget used to display snapshots
		// (this is added to the layout dynamically).
		mImageWidget = new Image();
		mImage = NULL;
	}

	Widget* getTopWidget()
	{
		Button* b = new Button();
		b->setText("Hello World");
		b->fillSpaceHorizontally();
		b->wrapContentVertically();
		return b;

		//return mImageWidget;
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
				maDestroyPlaceholder(mImage);
				mImage = NULL;
				return;
			}

			// Show the image in the image widget.
			mImageWidget->setTopPosition(400);
			mImageWidget->setLeftPosition(20);
			mImageWidget->setHeight(300);
			mImageWidget->setWidth(300);
			mImageWidget->setImage(mImage);
			mImageWidget->setScaleMode(NativeUI::IMAGE_SCALE_XY);
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
		createFocusListener();
		createUI();
		startCameraPreview();
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

		/*
		Button* b = new Button();
		b->setText("Take Snapshot");
		b->fillSpaceHorizontally();
		b->wrapContentVertically();
		mMainUI->addChild(b);
		*/

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
			((Button*) button)->setText("Hide Snapshot");
			showImage();
		}
		else
		{
			((Button*) button)->setText("Take Snapshot");
			hideImage();
		}
	}

	void showImage()
	{
		if (mSnapshotUI->imageIsHidden())
		{
			mSnapshotUI->obtainImage();
			mMainUI->addChild(mSnapshotUI->getTopWidget());
		}
	}

	void hideImage()
	{
		if (mSnapshotUI->imageIsVisible())
		{
			mSnapshotUI->releaseImage();
			mMainUI->removeChild(mSnapshotUI->getTopWidget());
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
