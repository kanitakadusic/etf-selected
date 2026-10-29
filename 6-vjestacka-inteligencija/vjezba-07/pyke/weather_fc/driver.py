from pyke import knowledge_engine, goal

engine = knowledge_engine.engine(__file__)
question = goal.compile("cinjenice.ponijeti($sta)")

def sta_ponijeti_sa_sobom():
    engine.reset()
    engine.activate("pravila")

    with question.prove(engine) as gen:
        for vars, plan in gen:
            print(f"Trebate ponijeti {vars['sta']}")
