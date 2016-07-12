load_package(rrl).
load_package(kbr).
load_package(blocks).
		
rrl_Qlearner(kbr).

rrl_numberOfEpisodes(1000).
rrl_saveQFunctions(1000).
rrl_testFrequency(50).
rrl_numberOfTests(100).

rrl_action_after_end_dangerous(yes).

%%%%%%% ACE BULLSHIT %%%%%%%%%%

rmode_key(model_info(MI)).
