i=0
while i< 1000000:
	i=i+1
	fichier = open("examplepy.out", "w")
	fichier.write("Hello X "+str(i))
	fichier.close() 
