#!/bin/bash

pfile=$(find . -type f -iname "*.psas")
/planner/verificationAsPlanning verify-all "$pfile" sas_plan >> verify.log
if [ -f sas_plan.verify ]; then # verifier sometimes shows unsolvability by reachability analysis
  /planner/pandaPIengine -g none --heuristic="rc2(add)" sas_plan.verify >> verify.log
  #if ! grep -q "\- Status\: Solved" verify.log; then
  #  /planner/dfs05 sas_plan.verify >> verify.log
  #  if ! grep -q "\- Status\: Solved" verify.log; then
  #    /planner/pproFF1800 sas_plan.verify >> verify.log
  #  fi
  #fi
fi
if grep -q "\- Status\: Solved" verify.log; then
  echo "TOAD: verification successful."
  exit 0
else
  echo "TOAD: verification FAILED."
  exit 105
fi
