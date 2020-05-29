import entity

entity.setName("John")
entity.setCallback("handler")


def Handler():

    entity.setName(entity.getName() + str(5))
    print(entity.getName())

