from pyke import knowledge_engine

engine = knowledge_engine.engine(__file__)

def sta_ponijeti_sa_sobom():
    engine.reset()
    engine.activate("pravila")

    with engine.prove_goal("pravila.ponijeti($sta)") as gen:
            for vars, plan in gen:
                print(f"Trebate ponijeti {vars['sta']}")
