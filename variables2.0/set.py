#crteando un conjunto con set
conjunto = set(["dato1","dato2"])

#metiendo un conjunto dentro de otro conjunto
conjunto1 = frozenset({"dato1", "dato2"})
conjunto2 = {conjunto1,"dato3"}

#teoria de conjuntos
setA = {1,3,5,7}
setB = {2,4,6,8}

#verificando si es un subconjunto
result1 = setB.issubset(setA)
result2 = setA.issuperset(setB)

#verificar si hay algun numero en comun
result3 = setB.isdisjoint(setA)

print(result3)