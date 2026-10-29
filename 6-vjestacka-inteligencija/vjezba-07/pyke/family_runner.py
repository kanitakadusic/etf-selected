import sys
import os

sys.path.append(os.path.join(os.path.dirname(__file__), "family_relations"))
import driver

print("\nTASK 1")
driver.get_relations_of("anne")

print("\nTASK 2")
driver.get_relation_between("bruce", "thomas")

print("\nTASK 3")
driver.get_people_whose_brother_is("m_thomas")

print("\nTASK 4")
driver.get_people_whose_sister_is("andrea")

print("\nTASK 5")
driver.get_people_whose_sibling_is("m_thomas")
driver.get_people_whose_sibling_is("andrea")

print("\nTASK 6")
driver.are_siblings("andrea", "jackie")
driver.are_siblings("andrea", "bruce")

print("\nTASK 7")
driver.get_grandparents_of("andrea")