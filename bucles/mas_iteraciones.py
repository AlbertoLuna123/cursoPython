frutas = ("Melon","Sandia","manzana","pera","mango","guanabana","banana")
cadena = "Hola mario alberto"
numeros = [2,5,4,6,3,7,6,8,9]

#SENTENCIA CONTINUE

for fruta in frutas:
    if fruta == "manzana":
        continue
    print(fruta)

print("---------------------------------------------")

#EVITAR QUE EL BUCLE SIGA EJECUTANDOSE
for fruta in frutas:
    if fruta == "pera":
        break
    print(fruta)
  
print("---------------------------------------------")
  
#ITERAR UNA CADENA DE TEXTO
for letra in cadena:
    print(letra)
    
print("---------------------------------------------")
print("Numeros duplicados")


#for en una linea de codigo
numeros_duplicados = [x*2 for x in numeros]

print(numeros_duplicados)
