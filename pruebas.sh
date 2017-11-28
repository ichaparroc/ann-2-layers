for $capa2 in {2..17}
	do
	for $capa3 in {2..17}
		do
		for i in 100 250 500 750 1000
			do
				./main.exe 0 8 $capa2 $capa3 1 0.05 1 entrenamiento.in
				./main.exe 1
			done
		echo $capa2 : $capa3
		done
done
