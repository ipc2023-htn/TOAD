//
// Created by dh on 01.05.21.
//

#ifndef FAST_DOWNWARD_DFA_DISTANCE_HEURISTIC_H
#define FAST_DOWNWARD_DFA_DISTANCE_HEURISTIC_H


#include "../heuristic.h"

namespace dfa_distance_heuristic {
    class DfaDistanceHeuristic : public Heuristic {
    private:
        int* hVals;
        int dfaIndex = -1;
    protected:
        int compute_heuristic(const State &s) override;

    public:
        DfaDistanceHeuristic(const options::Options &opts);
        ~DfaDistanceHeuristic();
    };
}

#endif //FAST_DOWNWARD_DFA_DISTANCE_HEURISTIC_H
