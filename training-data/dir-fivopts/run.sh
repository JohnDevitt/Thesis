

#!/usr/bin/env bash
 
SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"
echo "$SCRIPT_DIR"
cd $SCRIPT_DIR

echo "tilde" | $ACE_ILP_ROOT/bin/ace
echo "warmr" | $ACE_ILP_ROOT/bin/ace


