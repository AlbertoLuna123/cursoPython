#creando funcion divisor
def divisor():
    print("-----------------------------------")

divisor()
#crear funcion con parametro
def saludar(nombre,sexo):
    sexo = sexo.lower()
    if sexo == "hombre":
        print(f"hola {nombre}, mi amigo, como estas?")
    else:
        print(f"hola {nombre}, mi amiga, como estas?")

saludar("Marisol","Mujer")

divisor()

#crear una funcion que nos retorne valores
def crear_contrasena_5caracteres(num):
    chars = "abcdefghijhijklmnopqrstuvwxyz"
    num_entero = str(num)
    num = int(num_entero[0])
    c1 = num - 3
    c2 = num - 1
    c3 = num
    c4 = num + 1
    c5 = num + 3
    contrasena = f"{chars[c1]}{chars[c2]}{chars[c3]}{chars[c4]}{chars[c5]}{num*5}"
    return contrasena,num #creando el return para usar los valores de la contrasena fuera de la funcion, RETORNA UNA TUPLA
    
#desempaquetando la funcion en dos variables diferentes
contrasena, numero_utilizado= crear_contrasena_5caracteres(234)
print(f"tu contrasena es: {contrasena}")
print(f"El numero utilizado es: {numero_utilizado}")
    
    
    
    