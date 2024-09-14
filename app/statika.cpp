#include "statika.h"
#include<iostream>
#include<vector>
#include<utility>
#include<algorithm>


namespace statika {

    //Proracun reakcije na oslonac A u x pravcu
    double StaticEquilibrium::reactionOnSupportAx(const Force& force) {
        return FAx = force.Fx; // Reakcija na oslonac A u x pravcu

    };

    //Proracun reakcije na oslonac A u y pravcu
    double StaticEquilibrium::reactionOnSupportAy(const Beam& length, const Force& force, const Moment& moment, const UniformLoad& uniformLoad) {
        double L = length.L; // Duzina grede
        double q_length = uniformLoad.x2 - uniformLoad.x1; // Duzina kontinuiranog opterecenja
        double M_Fy = force.Fy * (L - force.Fy_x); // Moment sile u y pravcu na oslonacu B
        double M_q = uniformLoad.q * q_length * (L - (uniformLoad.x1 + q_length / 2)); // Moment kontinuiranog optereæenja u y pravcu na oslonacu B
        FAy = (M_Fy + M_q + moment.M) / L; // Reakcija na oslonac A u y pravcu
        return FAy;
    };

    //Proracun reakcije na oslonac B u y pravcu
    double StaticEquilibrium::reactionOnSupportBy(const Beam& length, const Force& force, const Moment& moment, const UniformLoad& uniformLoad) {
        double L = length.L; // Duzina grede
        double q_length = uniformLoad.x2 - uniformLoad.x1; // Duzina kontinuiranog opterecenja
        double M_Fy = force.Fy * force.Fy_x; // Moment sile u y pravcu na oslonacu A
        double M_q = uniformLoad.q * q_length * (uniformLoad.x1 + q_length / 2);// Moment kontinuiranog optereæenja u y pravcu na oslonacu A
        FBy = (M_Fy + M_q - moment.M) / L; // Reakcija na oslonac B u y pravcu
        return FBy;
    };

    //Grupiranje sila i momenata na gredi u parove po poziciji
    std::vector<std::pair<double, double>> StaticEquilibrium::collectForces(const Moment& moment, const Force& force, const UniformLoad& uniformLoad) {
        std::vector<std::pair<double, double>> forces;


        if (moment.M != 0.0) {
            forces.emplace_back(moment.M, moment.x); //kreiraj par (moment, pozicija momenta) i postaviga u vektor

        }

        if (force.Fy != 0.0) {
            forces.emplace_back(force.Fy, force.Fy_x); //kreiraj par (sila, pozicija sile) i postaviga u vektor

        }

        if (uniformLoad.q != 0.0) {
            forces.emplace_back(uniformLoad.q, uniformLoad.x1); //kreiraj par (opterecenje, pocetna pozicija opterecenja) i postaviga u vektor
            forces.emplace_back(uniformLoad.q, uniformLoad.x2); //kreiraj par (opterecenje, krajnja pozicija opterecenja) i postaviga u vektor

        }

        return forces;
    };


    //Sortiranje sila i momenata na gredi po poziciji
    void StaticEquilibrium::sortForces(std::vector<std::pair<double, double>>& forces) {
        std::sort(forces.begin(), forces.end(), [](std::pair<double, double> a, std::pair<double, double> b) {
            return a.second < b.second;
            });
    };

    //Podjela kontinuiranog optereæenja obziorm na položaj ostalih sila
    void StaticEquilibrium::adjustForcesWithUniformLoad(std::vector<std::pair<double, double>>& forces, const UniformLoad& uniformLoad) {
        std::vector<std::pair<double, double>> adjustments;

        for (size_t i = 0; i < forces.size(); ++i) {
            if (forces[i].second >= uniformLoad.x2) {
                break;
            }

            if (forces[i].second > uniformLoad.x1 && forces[i].second < uniformLoad.x2) {
                adjustments.emplace_back(uniformLoad.q, forces[i].second);

            }
        }

        for (const auto& adj : adjustments) {
            forces.push_back(adj);
        }

        sortForces(forces);
    }

    //Provjera da li je optereæenje samo kontinuirano
    bool StaticEquilibrium::isOnlyUniformLoad(const Moment& moment, const Force& force) {
        return (moment.M == 0 && force.Fy == 0);
    }

    //Provjera da li je optereæenje samo sila
    bool StaticEquilibrium::isOnlyForce(const Moment& moment, const UniformLoad& uniformLoad) {
        return (moment.M == 0 && uniformLoad.q == 0);
    }

    //Provjera da li je optereæenje samo moment
    bool StaticEquilibrium::isOnlyMoment(const UniformLoad& uniformLoad, const Force& force) {
        return (force.Fy == 0 && uniformLoad.q == 0);
    }

    //Proracun unutarnjeg momenta na gredi ako je optereæenje samo kontinuirano
    void StaticEquilibrium::calcMomentIfOnlyUniformLoad(const Beam& length, const UniformLoad& uniformLoad) {
        double L = length.L;
        internalMoments.emplace_back(0.0, 0.0);
        if (uniformLoad.x1 > 0 && uniformLoad.x2 < L) {
            double x1 = uniformLoad.x1;
            double x2 = uniformLoad.x2;
            double Mx1 = FAy * x1;
            double Mx2 = FBy * (L - x2);
            double Qx = uniformLoad.x1 + (uniformLoad.x2 - uniformLoad.x1) / 2;
            double Lq = uniformLoad.x2 - uniformLoad.x1;
            double Qx1 = uniformLoad.x1 + (Qx - uniformLoad.x1) / 2;
            double M_Qx = FAy * Qx - uniformLoad.q * (Lq / 2) * (Qx - Qx1);
            internalMoments.emplace_back(Mx1, x1);
            internalMoments.emplace_back(M_Qx, Qx);
            internalMoments.emplace_back(Mx2, x2);

        }

        else if (uniformLoad.x1 > 0.0 && uniformLoad.x2 == L) {
            double x1 = uniformLoad.x1;
            double Mx1 = FAy * x1;
            double Qx = uniformLoad.x1 + (uniformLoad.x2 - uniformLoad.x1) / 2;
            double Lq = uniformLoad.x2 - uniformLoad.x1;
            double Qx1 = uniformLoad.x1 + (Qx - uniformLoad.x1) / 2;
            double M_Qx = FAy * Qx - uniformLoad.q * (Lq / 2) * (Qx - Qx1);
            internalMoments.emplace_back(Mx1, x1);
            internalMoments.emplace_back(M_Qx, Qx);

        }

        else if (uniformLoad.x1 == 0.0 && uniformLoad.x2 < L) {
            double x2 = uniformLoad.x2;
            double Mx1 = FBy * (L - x2);
            double Qx = x2 / 2;
            double Lq = x2;
            double Qx2 = Qx + Qx / 2;
            double M_Qx = FBy * (L - Qx) - uniformLoad.q * (Lq / 2) * (Qx2 - Qx);
            internalMoments.emplace_back(M_Qx, Qx);
            internalMoments.emplace_back(Mx1, x2);

        }

        else {
            double M1 = uniformLoad.q * L * L / 8;
            internalMoments.emplace_back(M1, L / 2);

        }
        internalMoments.emplace_back(0.0, L);
    }
    //Proracun unutarnjeg momenta na gredi ako je optereæenje samo sila
    void StaticEquilibrium::calcMomentIfOnlyForce(const Beam& length, const Force& force) {
        double L = length.L;
        if (force.Fy_x != 0 || force.Fy_x == L) {
            double x1 = force.Fy_x;
            double Mx1 = FAy * x1;
            internalMoments.emplace_back(0.0, 0.0);
            internalMoments.emplace_back(Mx1, x1);
            internalMoments.emplace_back(0.0, L);


        }
    }

    //Proracun unutarnjeg momenta na gredi ako je optereæenje samo moment
    void StaticEquilibrium::calcMomentIfOnlyMoment(const Beam& length, const Moment& moment) {
        double L = length.L;
        if (moment.x == 0.0)
        {
            internalMoments.emplace_back(0.0, 0.0);
            internalMoments.emplace_back(-moment.M, 0.0);
            internalMoments.emplace_back(0.0, L);

        }

        else if (moment.x == L) {
            internalMoments.emplace_back(0.0, 0.0);
            internalMoments.emplace_back(moment.M, L);

        }

        else {
            double x = moment.x;
            double M = moment.M;
            double M1 = FAy * x;
            double M2 = M1 - M;
            internalMoments.emplace_back(0.0, 0.0);
            internalMoments.emplace_back(M1, x);
            internalMoments.emplace_back(M2, x);
            internalMoments.emplace_back(0.0, L);

        }
    }

    //Proracun unutarnjeg momenta na gredi ako greda optereæena silom i/ili momentom i/ili kontinuiranim optereæenjem
    void StaticEquilibrium::calcMoments(const std::vector<std::pair<double, double>>& forces, const Beam& length, const Moment& moment, const Force& force, const UniformLoad& uniformLoad) {
        double L = length.L;
        if (moment.x > 0 || moment.x < L) {
            internalMoments.emplace_back(0, 0);

            for (size_t i = 0; i < forces.size(); ++i) {

                double Mx1 = 0.0;
                double Mx2 = 0.0;

                Mx1 += FAy * forces[i].second;
                Mx1 += calcMomentFromPreviousForces(forces, i, force);
                Mx1 += calcMomentFromPreviousMoments(forces, i, moment);
                Mx1 += calcMomentFromPreviousUniformLoads(forces, i, uniformLoad);
                internalMoments.emplace_back(Mx1, forces[i].second);


                if (forces[i].first == moment.M)
                {
                    Mx2 = Mx1 - forces[i].first;
                    internalMoments.emplace_back(Mx2, forces[i].second);

                }
            }
            internalMoments.emplace_back(0, L);


        }

        else if (moment.x == 0.0)
        {
            for (size_t i = 1; i < forces.size(); ++i) {

                double Mx1 = 0.0;

                Mx1 += FAy * forces[i].second;
                Mx1 += calcMomentFromPreviousForces(forces, i, force);
                Mx1 += calcMomentFromPreviousMoments(forces, i, moment);
                Mx1 += calcMomentFromPreviousUniformLoads(forces, i, uniformLoad);
                internalMoments.emplace_back(Mx1, forces[i].second);


            }
        }

        else if (moment.x == L)
        {
            for (size_t i = 0; i < forces.size(); ++i) {

                double Mx1 = 0.0;

                Mx1 += FAy * forces[i].second;
                Mx1 += calcMomentFromPreviousForces(forces, i, force);
                Mx1 += calcMomentFromPreviousMoments(forces, i, moment);
                Mx1 += calcMomentFromPreviousUniformLoads(forces, i, uniformLoad);
                internalMoments.emplace_back(Mx1, forces[i].second);

            }
        }
    }

    //Proracun momenta od prethodnih sila
    double StaticEquilibrium::calcMomentFromPreviousForces(const std::vector<std::pair<double, double>>& forces, size_t i, const Force& force) {
        double Mx = 0.0;
        double x = forces[i].second;
        if (force.Fy_x < x) {
            Mx -= force.Fy * (x - force.Fy_x);
        }

        return Mx;
    };

    //Proracun momenta od prethodnih momenata
    double StaticEquilibrium::calcMomentFromPreviousMoments(const std::vector<std::pair<double, double>>& forces, size_t i, const Moment& moment) {
        double Mx = 0.0;
        if (moment.x < forces[i].second) {
            Mx -= moment.M;
        }
        return Mx;

    };

    //Proracun momenta od prethodnih kontinuiranih optereæenja
    double StaticEquilibrium::calcMomentFromPreviousUniformLoads(const std::vector<std::pair<double, double>>& forces, size_t i, const UniformLoad& uniformLoad) {
        double Mx = 0.0;
        double x = forces[i].second;


        if (x >= uniformLoad.x1 && x <= uniformLoad.x2) {
            double lengthWithinLoad = x - uniformLoad.x1;
            Mx -= (uniformLoad.q * lengthWithinLoad * lengthWithinLoad) / 2.0;
        }

        else if (x > uniformLoad.x2) {
            double lengthOfLoad = uniformLoad.x2 - uniformLoad.x1;
            double distanceToLoadCenter = x - (uniformLoad.x1 + lengthOfLoad / 2.0);
            Mx -= uniformLoad.q * lengthOfLoad * distanceToLoadCenter;
        }

        return Mx;
    };

    //Ciscenje unutarnjih momenata od duplikata
    void StaticEquilibrium::cleanInternalMoments(std::vector<std::pair<double, double>>& internalMoments) {
        for (auto it = internalMoments.begin(); it != internalMoments.end(); ++it) {
            for (auto jt = it + 1; jt != internalMoments.end();) {
                if (it->first == jt->first && it->second == jt->second) {
                    jt = internalMoments.erase(jt);
                }
                else {
                    ++jt;
                }
            }
        }
    }

    double StaticEquilibrium::getMaxMomentValue(const std::vector<std::pair<double, double>>& internalMoments) {
        if (internalMoments.empty()) {

            return 0;
        }

        std::pair<double, double> maxMoment = internalMoments[0];

        for (int i = 1; i < internalMoments.size(); i++) {
            if (internalMoments[i].first > maxMoment.first) {
                maxMoment = internalMoments[i];
            }
        }

        this->maxMoment.push_back(maxMoment);
        return maxMoment.first;

        /*double maxMoment = internalMoments[0].first; // Initialize with the first moment value

        // Iterate through the internal moments and find the maximum value
        for (const auto& moment : internalMoments) {
            if (moment.first > maxMoment) {
                maxMoment = moment.first; // Update maxMoment if a larger value is found
            }
        }

        return maxMoment;*/

    }



    //Proracun unutarnjeg momenta na gredi
    void StaticEquilibrium::calcInternalMoment(const Beam& length, const Moment& moment, const Force& force, const UniformLoad& uniformLoad) {

        std::vector<std::pair<double, double>> forces = collectForces(moment, force, uniformLoad);

        adjustForcesWithUniformLoad(forces, uniformLoad);

        if (isOnlyUniformLoad(moment, force)) {
            calcMomentIfOnlyUniformLoad(length, uniformLoad);
        }
        else if (isOnlyForce(moment, uniformLoad)) {
            calcMomentIfOnlyForce(length, force);
        }
        else if (isOnlyMoment(uniformLoad, force)) {
            calcMomentIfOnlyMoment(length, moment);
        }
        else {
            calcMoments(forces, length, moment, force, uniformLoad);
        }

        cleanInternalMoments(internalMoments);

    }

};

