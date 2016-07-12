#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

char *flags[] = {"-faggressive-loop-optimizations","-fauto-profile","-fauto-inc-dec","-fbranch-probabilities","-fbranch-target-load-optimize","-fbranch-target-load-optimize2","-fbtr-bb-exclusive","-fcaller-saves","-fcombine-stack-adjustments","-fconserve-stack","-fcompare-elim","-fcprop-registers","-fcrossjumping","-fcse-follow-jumps","-fcse-skip-blocks","-fcx-fortran-rules","-fcx-limited-range","-fdata-sections","-fdce","-fdelayed-branch","-fdelete-null-pointer-checks","-fdevirtualize","-fdevirtualize-speculatively","-fdevirtualize-at-ltrans","-fdse","-fearly-inlining","-fipa-sra","-fexpensive-optimizations","-ffat-lto-objects","-ffast-math","-ffinite-math-only","-ffloat-store","-fforward-propagate","-ffunction-sections","-fgcse","-fgcse-after-reload","-fgcse-las","-fgcse-lm","-fgraphite-identity","-fgcse-sm","-fhoist-adjacent-loads","-fif-conversion","-fif-conversion2","-findirect-inlining","-finline-functions","-finline-functions-called-once","-finline-small-functions","-fipa-cp","-fipa-cp-clone","-fipa-cp-alignment","-fipa-pta","-fipa-profile","-fipa-pure-const","-fipa-reference","-fipa-icf","-fira-hoist-pressure","-fira-loop-pressure","-fno-ira-share-save-slots","-fno-ira-share-spill-slots","-fisolate-erroneous-paths-dereference","-fisolate-erroneous-paths-attribute","-fivopts","-fkeep-inline-functions","-fkeep-static-consts","-flive-range-shrinkage","-floop-block","-floop-interchange","-floop-strip-mine","-floop-unroll-and-jam","-floop-nest-optimize","-floop-parallelize-all","-flra-remat","-flto","-fmerge-all-constants","-fmerge-constants","-fmodulo-sched","-fmodulo-sched-allow-regmoves","-fmove-loop-invariants","-fno-branch-count-reg","-fno-defer-pop","-fno-function-cse","-fno-guess-branch-probability","-fno-inline","-fno-math-errno","-fno-peephole","-fno-peephole2","-fno-sched-interblock","-fno-sched-spec","-fno-signed-zeros","-fno-toplevel-reorder","-fno-trapping-math","-fno-zero-initialized-in-bss","-fomit-frame-pointer","-foptimize-sibling-calls","-fpartial-inlining","-fpeel-loops","-fpredictive-commoning","-fprefetch-loop-arrays","-fprofile-correction","-fprofile-use","-fprofile-use=path","-fprofile-values","-fprofile-reorder-functions","-freciprocal-math","-free","-frename-registers","-freorder-blocks","-freorder-blocks-and-partition","-freorder-functions","-frerun-cse-after-loop","-freschedule-modulo-scheduled-loops","-frounding-math","-fsched2-use-superblocks","-fsched-pressure","-fsched-spec-load","-fsched-spec-load-dangerous","-fsched-critical-path-heuristic","-fsched-spec-insn-heuristic","-fsched-rank-heuristic","-fsched-last-insn-heuristic","-fsched-dep-count-heuristic","-fschedule-fusion","-fschedule-insns","-fschedule-insns2","-fselective-scheduling","-fselective-scheduling2","-fsel-sched-pipelining","-fsel-sched-pipelining-outer-loops","-fsemantic-interposition","-fshrink-wrap","-fsignaling-nans","-fsingle-precision-constant","-fsplit-ivs-in-unroller","-fsplit-wide-types","-fssa-phiopt","-fstdarg-opt","-fstrict-aliasing","-fstrict-overflow","-fthread-jumps","-ftracer","-ftree-bit-ccp","-ftree-builtin-call-dce","-ftree-ccp","-ftree-ch","-ftree-coalesce-vars","-ftree-copy-prop","-ftree-dce","-ftree-dominator-opts","-ftree-dse","-ftree-forwprop","-ftree-fre","-ftree-loop-if-convert","-ftree-loop-if-convert-stores","-ftree-loop-im","-ftree-phiprop","-ftree-loop-distribution","-ftree-loop-distribute-patterns","-ftree-loop-ivcanon","-ftree-loop-linear","-ftree-loop-optimize","-ftree-loop-vectorize","-ftree-pre","-ftree-partial-pre","-ftree-pta","-ftree-reassoc","-ftree-sink","-ftree-slsr","-ftree-sra","-ftree-switch-conversion","-ftree-tail-merge","-ftree-ter","-ftree-vectorize","-ftree-vrp","-funit-at-a-time","-funroll-all-loops","-funroll-loops","-funsafe-loop-optimizations","-funsafe-math-optimizations","-funswitch-loops","-fipa-ra","-fvariable-expansion-in-unroller","-fvect-cost-model","-fvpt","-fweb","-fwhole-program","-fuse-linker-plugin"};


int buildBaseline();
double getRandomProbability();
void buildCompileCommand();
int timedRunProgram();


int main ( int argc, char *argv[] )
{
	srand(time(0));
	int baseline = buildBaseline(argv[1]);

    int shortest = INT_MAX;
    int longest = 0;

    char bestCompileCommand[65536];
    char worstCompileCommand[65536];
    char bestAceInput[65536];
    char worstAceInput[65536];

	int i;
	for(i = 0; i < 1000; i++) {
		char currentCompileCommand[65536];
		char currentAceInput[65536];

		buildCompileCommand(currentCompileCommand, currentAceInput, 65536, argv[1]);
		int status = system(currentCompileCommand);

		if(status == 0) {
			char runCommand[1024];
			snprintf(runCommand, sizeof(runCommand), "%s/%s", argv[1], argv[1]);
			int runTime = timedRunProgram(runCommand);

			if(runTime < shortest) {
				shortest = runTime;
				snprintf(bestCompileCommand, sizeof(bestCompileCommand), "%s", currentCompileCommand);
				snprintf(bestAceInput, sizeof(bestAceInput), "%s", currentAceInput);
			} else if(runTime > longest) {
				longest = runTime;
				snprintf(worstCompileCommand, sizeof(worstCompileCommand), "%s", currentCompileCommand);
				snprintf(worstAceInput, sizeof(worstAceInput), "%s", currentAceInput);
			}
		}
	}


	char file[1024];
	snprintf(file, sizeof(file), "%s/iterativeOutput.txt", argv[1]);
	FILE *fp = fopen(file, "w+");


	fprintf(fp, "\n\n\n");
	fprintf(fp, "Basleine: %d\n", baseline);
	fprintf(fp, "Shortest: %d\n", shortest);
	fprintf(fp, "Longest: %d\n", longest);
	fprintf(fp, "--------------------------------------\n\n");
	fprintf(fp, "Best Compile Command:\n\n");
	fprintf(fp, "%s\n\n", bestCompileCommand);
	fprintf(fp, "Best Ace Input:\n\n");
	fprintf(fp, "%s\n\n", bestAceInput);
	fprintf(fp, "\n\n\n");



	fclose(fp);
}




int buildBaseline(char *arg) {
	char compileCommand[1024];
	snprintf(compileCommand, 1024, "make %s clean; make -C %s", arg, arg);
	system(compileCommand);

	char runCommand[1024];
	snprintf(runCommand, sizeof(runCommand), "%s/%s", arg, arg);
	return timedRunProgram(runCommand);
}



double getRandomProbability() {
    return (double)rand() / (double)RAND_MAX ;
}

void buildCompileCommand(char *compileCommand, char *aceInput, int size, char *arg) {

	double random = getRandomProbability();
    char *optimisationFlag;

    if( random <= 0.3333333333 ) {
    	optimisationFlag = "-O1";
    } else if( random > 0.3333333333 && random <= 0.6666666666 ) {
    	optimisationFlag = "-O2";
    } else {
    	optimisationFlag = "-O3";
    }


    snprintf(compileCommand, size, "make -C %s clean; make -C %s CFLAGS='%s", arg, arg, optimisationFlag);
    snprintf(aceInput, size, "begin(model(program1)).");

	int i;
    for(i = 0; i < sizeof(flags) / sizeof(char*); i++)
	{
    	if(getRandomProbability() <= 0.1) {
    		strcat(compileCommand, " ");
    		strcat(compileCommand, flags[i]);

    		strcat(aceInput, "\n");
    		strcat(aceInput, flags[i]);
    	}
	}

	strcat(compileCommand, "'");
	strcat(aceInput, "\nend(model(machine1)).");
}

int timedRunProgram(char* runCommand) {
	clock_t start = clock(), diff;
	system(runCommand);
	diff = clock() - start;

	return (int)diff;
}
