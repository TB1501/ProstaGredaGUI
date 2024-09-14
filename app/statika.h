#pragma once
#include <iostream>
#include <vector>
#include<utility>

namespace statika {


	struct Moment {
		double M = 0; // Moment na gredi
		double x = 0; // Pozicija momenta na gredi
	};

	struct Force {
		double Fx = 0; // Sila na gredi u x pravcu
		double Fy = 0; // Sila na gredi u y pravcu
		double Fx_x = 0; // Pozicija sile u x pravcu na gredi
		double Fy_x = 0; // Pozicija sile u y pravcu na gredi
	};

	struct UniformLoad {

		double q = 0; // Uniformno rasporeðeno optereæenje
		double x1 = 0; // Poèetna pozicija kontinuiranog optereæenja
		double x2 = 0; // Krajnja pozicija kontinuiranog optereæenja
	};

	struct Beam {
		double L = 0; // Duzina grede
	};


	class StaticEquilibrium {

		double FAx = 0; // Reakcija na oslonac A u x pravcu
		double FAy = 0; // Reakcija na oslonac A u y pravcu
		double FBy = 0; // Reakcija na oslonac B u y pravcu
		std::vector<std::pair<double, double>>internalMoments; // Unutarnji momenti na gredi
		std::vector<std::pair<double, double>>internalForces_y;// Unutarnje sile u y pravcu na gredi
		std::vector<std::pair<double, double>>internalForces_x;// Unutranje sile u x pravcu na gredi
		std::vector<std::pair<double, double>>maxMoment;
		std::vector<std::pair<double, double>> collectForces(const Moment& moment, const Force& force, const UniformLoad& uniformLoad);
		void sortForces(std::vector<std::pair<double, double>>& forces);
		void adjustForcesWithUniformLoad(std::vector<std::pair<double, double>>& forces, const UniformLoad& uniformLoad);
		bool isOnlyUniformLoad(const Moment& moment, const Force& force);
		bool isOnlyForce(const Moment& moment, const UniformLoad& uniformLoad);
		bool isOnlyMoment(const UniformLoad& uniformLoad, const Force& force);
		void calcMomentIfOnlyUniformLoad(const Beam& length, const UniformLoad& uniformLoad);
		void calcMomentIfOnlyForce(const Beam& length, const Force& force);
		void calcMomentIfOnlyMoment(const Beam& length, const Moment& moment);
		void calcMoments(const std::vector<std::pair<double, double>>& forces, const Beam& length, const Moment& moment, const Force& force, const UniformLoad& uniformLoad);
		double calcMomentFromPreviousForces(const std::vector<std::pair<double, double>>& forces, size_t i, const Force& force);
		double calcMomentFromPreviousMoments(const std::vector<std::pair<double, double>>& forces, size_t i, const Moment& moment);
		double calcMomentFromPreviousUniformLoads(const std::vector<std::pair<double, double>>& forces, size_t i, const UniformLoad& uniformLoad);
	public:
		double get_FAx() const { return FAx; };
		double get_FAy() const { return FAy; };
		double get_FBy() const { return FBy; };
		double reactionOnSupportAx(const Force& force = Force());
		double reactionOnSupportAy(const Beam& legth = Beam(), const Force& force = Force(), const Moment& moment = Moment(), const UniformLoad& uniformLoad = UniformLoad());
		double reactionOnSupportBy(const Beam& legth = Beam(), const Force& force = Force(), const Moment& moment = Moment(), const UniformLoad& uniformLoad = UniformLoad());
		void calcInternalMoment(const Beam& length, const Moment& moment = Moment(), const Force& force = Force(), const UniformLoad& uniformLoad = UniformLoad());
		const std::vector<std::pair<double, double>>& getInternalMoments() const { return internalMoments; };
		void cleanInternalMoments(std::vector<std::pair<double, double>>& internalMoments);
		double getMaxMomentValue(const std::vector<std::pair<double, double>>& internalMoments);


	};



};

