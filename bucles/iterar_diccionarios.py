diccionario = {
    'nombre':"Mario",
    'apellido': "Luna",
    'edad':25,
    'sexo':"Masculino"
}

#RECORRIENDO DICCIONARIO
for key in diccionario:
    print(key)

print("------------------------------------------")  

  
#RECORRIENDO DICCIONARIO CON .items()
for values in diccionario.items():
    keys = values[0]
    val = values[1]
    print(f"llave: {keys}, valor: {val}")
    