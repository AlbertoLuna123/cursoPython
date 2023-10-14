#creando tupla de animales
animales = ["pez","perro","gato","loro","cocodrilo"]

#creando tupla de numeros
numeros = [10,20,30,40,50]

#imprimiendo tupla de animales
for animal in animales:
    print(animal)

print("---------------------------------------------")
    
#imprimiendo tupla de numeros dividido entre 10
for numero in numeros:
    resul = numero /10
    print(resul)
    
print("---------------------------------------------")

#imprimendo 2 tuplas al mismo tiempo con la funcion zip
for numero,animal in zip(animales,numeros):
    print(numero)
    print(animal)
   
print("---------------------------------------------")
 
#ITERANDO CON LA FUNCION RANGE
for rango in range(1,100):
    print(rango)
    
print("---------------------------------------------")

#ITERANDO CON LA FUNCION ENUMERATE
for enum in enumerate(animales):
    print(enum)
    pos = enum[0]
    val = enum[1]
    print(f"posicion: {pos}, valor: {val}")
    

    
print("---------------------------------------------")
#ITERANDO CON FOR ELSE

tupla = list()
print(tupla)
  
for num in tupla:
    print(num)
else:
    print("No hay elementos en la tupla")