//
// Header only c++ math wrapper to reduce verbosity
//

#pragma once

#include <random>
#include <string>

namespace Math {
	// private namespace members so to say
	namespace Detail {
		static std::mt19937 randGen(std::random_device{}());
	}

    static double eulersNumber = 2.7182818;
	static double PI = 3.14159265;

	template<class T>
	static T SigmoidFunction(const T& input) { return 1.0 / (1 + pow(eulersNumber, -input)); }

	template<class T>
	static T RandomUniform(const T& min, const T& max) { return std::uniform_real_distribution<T>(min, max)(Detail::randGen); }

	template<>
	static int RandomUniform<int>(const int& min, const int& max) { return std::uniform_int_distribution<int>(min, max)(Detail::randGen); }
	
	template<class T>
	static T RandomNormal(const T& mean, const T& stdDev) { return std::normal_distribution<T>(mean, stdDev)(Detail::randGen); }
};
