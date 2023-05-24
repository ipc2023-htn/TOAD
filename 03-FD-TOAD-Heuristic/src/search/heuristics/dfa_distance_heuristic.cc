//
// Created by dh on 01.05.21.
//

#include <fstream>
#include "dfa_distance_heuristic.h"
#include "../option_parser.h"
#include "../plugin.h"

using namespace std;

namespace dfa_distance_heuristic {

    DfaDistanceHeuristic::DfaDistanceHeuristic(const options::Options &opts) : Heuristic(opts) {
        utils::g_log << "Initializing DFA distance heuristic..." << endl;
        string filename = "./dfad.heuristic";
        ifstream is;
        is.open(filename);
        int i = 0;
        int numStates = -1;
        if (is.is_open()) {
            is >> numStates;
            hVals = new int[numStates];
            int hVal;
            while (is >> hVal) {
                hVals[i++] = hVal;
            }
            is.close();
        } else {
            utils::g_log << "Unable to open file \"" << filename << "\"";
            exit(-17);
        }
        dfaIndex = task_proxy.get_variables().size() - 1;
        auto v = task_proxy.get_variables()[dfaIndex];
        if (v.get_domain_size() != numStates) {
            utils::g_log << "ERROR: Heuristic definition is invalid.\n";
            utils::g_log << "- got " << to_string(numStates) + " h values for " << to_string(v.get_domain_size()) + " states.\n";
            exit(-17);
        }
    }

    DfaDistanceHeuristic::~DfaDistanceHeuristic() {
        delete[] hVals;
    }

    int DfaDistanceHeuristic::compute_heuristic(const State &s) {
        s.unpack();
        int val = s.get_unpacked_values()[dfaIndex];
        int hVal = hVals[val];
        if (hVal < 0) {
            utils::g_log << "ERROR implausible DFAD heuristic value for state " << val << " -> " << hVal << endl;
            exit(-17);
        }
        return hVal;
    }

    static shared_ptr<Heuristic> _parse(OptionParser &parser) {
        parser.document_synopsis("DAF distance heuristic",
                                 "Returns distance in TOAD DFA");
        parser.document_language_support("action costs", "not supported");
        parser.document_language_support("conditional effects", "supported");
        parser.document_language_support("axioms", "supported");
        parser.document_property("admissible", "yes");
        parser.document_property("consistent", "yes");
        parser.document_property("safe", "yes");
        parser.document_property("preferred operators", "no");

        Heuristic::add_options_to_parser(parser);
        Options opts = parser.parse();
        if (parser.dry_run())
            return nullptr;
        else
            return make_shared<DfaDistanceHeuristic>(opts);
    }

    static Plugin<Evaluator> _plugin("dfad", _parse);
}
