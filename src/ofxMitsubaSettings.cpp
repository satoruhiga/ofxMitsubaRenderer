#include "ofxMitsubaSettings.h"

namespace ofxMitsuba
{

Settings::Settings()
{
}

Settings::Settings(string integrator, string sampler) : integrator(integrator), sampler(sampler)
{
}

Settings::Settings(const Settings &copy)
{
	integrator = copy.getIntegrator();
	sampler = copy.getSampler();
}

Settings DirectIllumination(int sampleCount)
{
	return Settings(Integrator("direct", sampleCount > 64, sampleCount < 1, "<integer name=\"bsdfSamples\" value=\"8\"/><integer name=\"luminaireSamples\" value=\"8\"/>"),
					Sampler("ldsampler", sampleCount));
}

Settings PathTracer(int sampleCount)
{
	return Settings(Integrator("path", sampleCount > 32, sampleCount < 1),
					Sampler("ldsampler", sampleCount));
}

Settings PhotonMapper(int sampleCount)
{
	return Settings(Integrator("photonmapper", sampleCount > 32, sampleCount < 1),
					Sampler("ldsampler", sampleCount));
}
	
}
