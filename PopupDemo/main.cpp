#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <NativeUI/Widgets.h>
#include <NativeUI/WidgetUtil.h>

using namespace MAUtil;
using namespace NativeUI;

/**
 * Class that creates to screen and main layout.
 * The idea is that by having this as a separate
 * class, different UI components can be added to
 * it in a modular way.
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

	Widget* getMainLayout()
	{
		return mLayout;
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
 * Class that contains buttons and
 * widgets dynamically shown/hidden.
 */
class PopupUI : public ButtonListener
{
public:
	/**
	 * The parent is the layout to add widgets to.
	 */
	PopupUI(Widget* parent)
	{
		createUI(parent);
	}

	void createUI(Widget* parent)
	{
		mParent = parent;

	    mStaticButton = new Button();
	    mStaticButton->setText("Touch Me To Show Popup");
	    mStaticButton->setTopPosition(50);
	    mStaticButton->setLeftPosition(50);
	    mStaticButton->setHeight(300);
	    mStaticButton->setWidth(300);
	    mStaticButton->setAlpha(1.0);
	    mStaticButton->setBackgroundColor(0x0000FF);
	    mStaticButton->setFontColor(0xFFFFFF);
	    mStaticButton->addButtonListener(this);
	    mParent->addChild(mStaticButton);

	    mPopupButton = new Button();
	    mPopupButton->setText("Touch To Hide Me");
	    mPopupButton->setTopPosition(100);
	    mPopupButton->setLeftPosition(300);
	    mPopupButton->setHeight(300);
	    mPopupButton->setWidth(300);
	    mPopupButton->setAlpha(1.0);
	    mPopupButton->setBackgroundColor(0x008800);
	    mPopupButton->setFontColor(0xFFFFFF);
	    mPopupButton->addButtonListener(this);
	}

	void buttonClicked(Widget* button)
	{
		if (button == mStaticButton)
		{
			mParent->addChild(mPopupButton);
		}
		else
		if (button == mPopupButton)
		{
			mParent->removeChild(mPopupButton);
		}
	}

private:
    Button* mStaticButton;
    Button* mPopupButton;
    Widget* mParent;
};


/**
 * A slightly different Popup UI module.
 */
class PopupUI2 : public ButtonListener
{
public:
	/**
	 * The parent is the layout to add widgets to.
	 */
	PopupUI2(Widget* parent)
	{
		createUI(parent);
	}

	void createUI(Widget* parent)
	{
		mParent = parent;

		mPopupIsVisible = false;

	    mStaticButton = new Button();
	    mStaticButton->setTopPosition(400);
	    mStaticButton->setLeftPosition(50);
	    mStaticButton->setHeight(300);
	    mStaticButton->setWidth(300);
	    mStaticButton->setAlpha(1.0);
	    mStaticButton->setBackgroundColor(0x0000FF);
	    mStaticButton->setFontColor(0xFFFFFF);
	    mStaticButton->addButtonListener(this);
	    mParent->addChild(mStaticButton);
	    updateStaticButtonText();

	    mPopupButton = new Button();
	    mPopupButton->setText("Hello World :)");
	    mPopupButton->setTopPosition(450);
	    mPopupButton->setLeftPosition(300);
	    mPopupButton->setHeight(300);
	    mPopupButton->setWidth(300);
	    mPopupButton->setAlpha(1.0);
	    mPopupButton->setBackgroundColor(0x008800);
	    mPopupButton->setFontColor(0xFFFFFF);
	    mPopupButton->addButtonListener(this);
	}

	void updateStaticButtonText()
	{
		if (mPopupIsVisible)
		{
			mStaticButton->setText("Hide Popup");
		}
		else
		{
			mStaticButton->setText("Show Popup");
		}
	}

	void buttonClicked(Widget* button)
	{
		if (mPopupIsVisible)
		{
			mParent->removeChild(mPopupButton);
			mPopupIsVisible = false;
			updateStaticButtonText();
		}
		else
		{
			mParent->addChild(mPopupButton);
			mPopupIsVisible = true;
			updateStaticButtonText();
		}
	}

private:
    Button* mStaticButton;
    Button* mPopupButton;
    Widget* mParent;
    bool mPopupIsVisible;
};

/**
 * Simple test application for the popup demo.
 */
class PopupDemoMoblet : public Moblet
{
public:
	PopupDemoMoblet()
	{
		createUI();
	}

	void createUI()
	{
		// Create and show UI components.
		mMainUI = new MainUI();
		mPopupUI = new PopupUI(mMainUI->getMainLayout());
		mPopupUI2 = new PopupUI2(mMainUI->getMainLayout());
		mMainUI->show();
	}

	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			close();
		}
	}

private:
    MainUI* mMainUI;
    PopupUI* mPopupUI;
    PopupUI2* mPopupUI2;
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new PopupDemoMoblet());
	return 0;
}
