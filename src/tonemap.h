#ifndef _TONEMAP_H_
#define _TONEMAP_H_

enum class TonemapMode { PHOTOGRAPHIC, NOTONEMAP };

class Tonemap
{
	public:

	TonemapMode tonemap_mode;
	float a;	// Key value
	float burnout;
	float saturation;
	float gamma;

	Tonemap(TonemapMode _tonemap_mode=TonemapMode::NOTONEMAP, float _a=0.18f,
			float _burnout=1.0f, float _saturation=1.0f, float _gamma=2.2f) :
	tonemap_mode(_tonemap_mode), a(_a), burnout(_burnout),
	saturation(_saturation), gamma(_gamma) {}

	Tonemap& operator=(const Tonemap& rhs)
	{
		tonemap_mode = rhs.tonemap_mode;
		a 	     = rhs.a;
		burnout      = rhs.burnout;
		saturation   = rhs.saturation;
		gamma 	     = rhs.gamma;

		return *this;
	}
};

#endif
