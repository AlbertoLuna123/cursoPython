#forma no optima de sumar valores
#def suma(lista):
#    numeros_sumados = 0
#    for numero in lista:
#        numeros_sumados = numeros_sumados + numero
#    return numeros_sumados

#resultado = suma([1,4,2,3,5,4,6,6,7,4,45,4,564,56,4])

#utilizando parametro argas

def suma(*numeros):
    return sum(numeros)

resultado = suma(1,2,3,2,3,4,3,4,5,4,5,45,4,3,45,4)
print(resultado)

