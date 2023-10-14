#creacndo diccionarios con la funcion dict

diccionario = dict(nombre='Mario', apellido='Luna')

#las listas no pueden ser claves y usamos frozen set para meter conjuntos
diccionario = {frozenset(["ola","mi nombre","comida"]):"bienvenido"}

#creando diccionario con fromkes
diccionario = dict.fromkeys(["nombre", "apellido"])
print (diccionario)
