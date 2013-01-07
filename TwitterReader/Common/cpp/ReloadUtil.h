#include <Wormhole/HybridMoblet.h>

// A simple low pass filter used to
// smoothen noisy accelerometer data.
struct LowPassFilter
{
	/**
	 * Default constructor.
	 * Sets the cutoff to a default value.
	 */
	LowPassFilter() :
		mCutOff(0.80f)
	{
	}

	/**
	 * Constructor.
	 * @param cutoff Sets the cutoff for the filter.
	 * It must be a value between 0 and 1, where
	 * 0 means no filtering (everything is passed through)
	 * and 1 that no signal is passed through.
	 */
	LowPassFilter(float cutoff) :
		mCutOff(cutoff)
	{
	}

	float filter(float in)
	{
		mPreviousState =
			(in * (1.0f - mCutOff)) +
			(mPreviousState * mCutOff);
		return mPreviousState;
	}

	float mCutOff;
	float mPreviousState;
};

class ReloadUtil : public MAUtil::SensorListener
{
public:
	ReloadUtil();
	virtual ~ReloadUtil();
	//void useSensorToReload();
	//void useKeyToReload(int keyCode);
	virtual void setReloadURL(const MAUtil::String& url);
	virtual void setMoblet(Wormhole::HybridMoblet* moblet);
	int startSensor();
	int stopSensor();
	void sensorEvent(MASensor sensor);
	virtual void reload();

protected:
	MAUtil::String mReloadURL;
	Wormhole::HybridMoblet* mMoblet;
	LowPassFilter mFilter;
	float mPreviousSensorValue;
	float mAccumulatedSensorValue;
};

ReloadUtil::ReloadUtil() :
	mFilter(0.3f),
	mPreviousSensorValue(0.0f),
	mAccumulatedSensorValue(0.0f)
{
	mMoblet = NULL;
	Environment::getEnvironment().addSensorListener(this);
	startSensor();
}

ReloadUtil::~ReloadUtil()
{
	stopSensor();
	Environment::getEnvironment().removeSensorListener(this);
}

void ReloadUtil::setReloadURL(const MAUtil::String& url)
{
	mReloadURL = url;
}

void ReloadUtil::setMoblet(Wormhole::HybridMoblet* moblet)
{
	mMoblet = moblet;
}

int ReloadUtil::startSensor()
{
	return maSensorStart(SENSOR_TYPE_ACCELEROMETER, SENSOR_RATE_UI);
}

int ReloadUtil::stopSensor()
{
	return maSensorStop(SENSOR_TYPE_ACCELEROMETER);
}

#define FABS(n) (((n) < 0.0) ? ((n) * -1.0) : (n))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

void ReloadUtil::sensorEvent(MASensor sensor)
{
	// If sensor data received is from the accelerometer...
	if (sensor.type == SENSOR_TYPE_ACCELEROMETER)
	{
		//float sensorValue = mFilter.filter(sensor.values[0]);
		float sensorValue =sensor.values[0];
		float sensorDelta = sensorValue - mPreviousSensorValue;
		mPreviousSensorValue = sensorValue;
		mAccumulatedSensorValue += FABS(sensorDelta);
		mAccumulatedSensorValue -= 0.05f;
		mAccumulatedSensorValue = MAX(0.0f, mAccumulatedSensorValue);

		MAUtil::String accum = "";
		for (float i = 0.0; i < mAccumulatedSensorValue; i += 0.1)
		{
			accum += "**";
		}
		//lprintfln("@@@ Accum: %f", mAccumulatedSensorValue);
		lprintfln("@@@ Accum: %s", accum.c_str());

		// Reload if threshold is reached.
		if (mAccumulatedSensorValue > 1.0f)
		{
			lprintfln("@@@ FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE");
			mAccumulatedSensorValue = 0.0;
			reload();
		}
	}
}

void ReloadUtil::reload()
{
	if (NULL != mMoblet)
	{
		mMoblet->showPage(mReloadURL);
	}
}
