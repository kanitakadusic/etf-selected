from pyke import knowledge_engine, goal

engine = knowledge_engine.engine(__file__)
fc_goal = goal.compile("family.how_related($person1, $person2, $relationship)")

def get_relations_of(person1):
    engine.reset()
    engine.activate("fc_example")

    with fc_goal.prove(engine, person1=person1) as gen:
        for vars, plan in gen:
            print(f"{person1, vars['person2']} : {vars['relationship']}")

def get_relation_between(person1, person2):
    engine.reset()
    engine.activate("fc_example")

    with fc_goal.prove(engine, person1=person1, person2=person2) as gen:
        for vars, plan in gen:
            print(f"{person1, person2} : {vars['relationship']}")

def get_people_whose_brother_is(person1):
    engine.reset()
    engine.activate("fc_example")

    with fc_goal.prove(engine, person1=person1) as gen:
        for vars, plan in gen:
            if "brother" in str(vars['relationship']):
                print(f"{person1, vars['person2']} : {vars['relationship']}")

def get_people_whose_sister_is(person1):
    engine.reset()
    engine.activate("fc_example")

    with fc_goal.prove(engine, person1=person1) as gen:
        for vars, plan in gen:
            if "sister" in str(vars['relationship']):
                print(f"{person1, vars['person2']} : {vars['relationship']}")

def get_people_whose_sibling_is(person1):
    engine.reset()
    engine.activate("fc_example")

    with fc_goal.prove(engine, person1=person1) as gen:
        for vars, plan in gen:
            if "sister" in str(vars['relationship']) or "brother" in str(vars['relationship']):
                print(f"{person1, vars['person2']} : {vars['relationship']}")

def are_siblings(person1, person2):
    engine.reset()
    engine.activate("fc_example")

    with fc_goal.prove(engine, person1=person1, person2=person2) as gen:
        for vars, plan in gen:
            if "sister" in str(vars['relationship']) or "brother" in str(vars['relationship']):
                print("true")

def get_grandparents_of(person1):
    engine.reset()
    engine.activate("fc_example")

    with fc_goal.prove(engine, person1=person1) as gen:
        for vars, plan in gen:
            if "grand" in str(vars['relationship']) and "great" not in str(vars['relationship']):
                print(f"{person1, vars['person2']} : {vars['relationship']}")