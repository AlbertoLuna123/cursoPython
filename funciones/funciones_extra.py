def frase(nombre, apellido,adjetivo):
    return f"Hola {nombre} {apellido}, eres muy {adjetivo}"

#forzar argumentos
frase_resul = frase(adjetivo = "pendejo",apellido = "Luna",nombre="Mario")
print(frase_resul)