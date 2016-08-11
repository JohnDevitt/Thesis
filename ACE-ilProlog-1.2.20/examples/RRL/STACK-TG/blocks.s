%:mode=prolog:

load_package(blocks_world).
load_package(user_environment).
%load_package(blocks).
load_package(rrl).
load_package(irl).
load_package(tg).
load_package(rrl3).

randomise(time).

%execute(mrrl3(5)).
%execute(q).

number_of_blocks(7).
blocks_world_goal(stack).

rrl_Qlearner(tg).

tg_min_sample_size(50).

rrl_Plearner(off).
rrl_startPlearnerAt(500).


rrl_numberOfEpisodes(5000).
rrl_saveQFunctions(200).
rrl_testFrequency(50).
rrl_numberOfTests(100).

rrl_teacherFrequency(off).
rrl_askQuestions(yes).
rrl_numberOfQuestions(5).

rrl_output_options([qfunction(50),results]).

type(model_info(modelinfo)).
rmode_key(model_info(MI)).

type(get_model(modelinfo,state,block,block)).
root( (model_info(MI),
          get_model(MI,State,X,Y),
	  %goal_stack(State),
	  goal_on(State,A,B),
	  numberofblocks(State,Number)) ).

typed_language(yes).
type(clear(state,block)).
type(on(state,block,block)).
type(eq(block,block)).
type(above(state,block,block)).
type(height(state,block,number)).
type(numberofblocks(state,number)).
type(numberofstacks(state,number)).
type(number < number).
type(number = number).
type(diff_number(number,number,number)).
type(goal_on(state,block,block)).
type(goal_stack(state)).
type(goal_unstack(state)).
type(goal_clear(state,block)).
type(member(number,list)).
type(false).

rmode(10: clear(+State,+-X)).
rmode(10: on(+State,+-X,+-Y)).
rmode(10: on(+State,+-X, floor)).
rmode(10: eq(+X,+Y)).
rmode(10: eq(+X,floor)).
rmode(10: false).
rmode(10: above(+State,+-X,+-Y)).
rmode(10: (height(+State,+-X,-H), height(+State,+-X2,-H2), H < H2)).
rmode(10: (height(+State,+-X,-H), height(+State,+-X2,-H2), H2 < H)).
rmode(10: (height(+State,+-X,-H), diff_number(+N,H,-D), height(+State,+-X2,-H2), diff_number(N,H2,-D2), D < D2)).
rmode(10: (height(+State,+-X,-H), diff_number(+N,H,-D), height(+State,+-X2,-H2), diff_number(N,H2,-D2), D2 < D)).
rmode(10: #(C: member(C,[0,1,2,3,4,5,6,7,8,9,10]), (height(+State,+-X,-H), H = C))).
rmode(10: #(C: member(C,[0,1,2,3,4,5,6,7,8,9,10]), (height(+State,+-X,-H), H < C))).
rmode(10: #(C: member(C,[0,1,2,3,4,5,6,7,8,9,10]), (height(+State,+-X,-H), diff_number(+N,H,-D), D = C))).
rmode(10: #(C: member(C,[0,1,2,3,4,5,6,7,8,9,10]), (height(+State,+-X,-H), diff_number(+N,H,-D), D < C))).
%rmode(10: #(C: member(C,[0,1,2,3,4,5,6,7,8,9,10]), (numberofstacks(+State,-S), S = C))).
%rmode(10: #(C: member(C,[0,1,2,3,4,5,6,7,8,9,10]), (numberofstacks(+State,-S), S < C))).
%rmode(10: #(C: member(C,[0,1,2,3,4,5,6,7,8,9,10]), (numberofstacks(+State,-S), diff_number(+N,S,-D), D = C))).
%rmode(10: #(C: member(C,[0,1,2,3,4,5,6,7,8,9,10]), (numberofstacks(+State,-S), diff_number(+N,S,-D), D < C))).


