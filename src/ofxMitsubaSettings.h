#pragma once

#include "ofMain.h"

namespace ofxMitsuba
{

struct Sampler
{
	string type;
	int sampleCount;
	
	Sampler(string type, int sampleCount = 4)
	: type(type), sampleCount(sampleCount) {}
	
	operator string() const
	{
		ostringstream oss;
		oss << "<sampler type='" << type << "'>";
		oss << "<integer name='sampleCount' value='" << sampleCount << "'/>";
		oss << "</sampler>";
		return oss.str();
	}
};

struct Integrator
{
	string type;
	bool adaptive;
	bool use_cache;
	string option;
	
	Integrator(string type, bool use_cache = false, bool adaptive = false, string option = "")
	: type(type), use_cache(use_cache), adaptive(adaptive), option(option) {}
	
	operator string() const
	{
		ostringstream oss;
		if (adaptive) oss << "<integrator type='errctrl'>";
		if (use_cache) oss << "<integrator type='irrcache'>";
		oss << "<integrator type='" << type << "'>";
		if (!option.empty()) oss << option;
		oss << "</integrator>";
		if (use_cache) oss << "</integrator>";
		if (adaptive) oss << "</integrator>";
		return oss.str();
	}
};
	
class Settings
{
public:
	
	Settings();
	Settings(string integrator, string sampler);
	Settings(const Settings &copy);
	
	string getIntegrator() const { return integrator; }
	string getSampler() const { return sampler; }
	
protected:
	
	string integrator, sampler;
	
};
	
Settings DirectIllumination(int sampleCount = 16);
Settings PathTracer(int sampleCount = 4);
Settings PhotonMapper(int sampleCount = 4);
	
}