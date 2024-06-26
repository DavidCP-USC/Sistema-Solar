float verticesISS[] = {
	//Cubo central
	//Cara frontal
	-0.4f, -0.4f, -0.4f,  0.0f, 0.66f, 0.0f,  0.0f, -1.0f,
	-0.4f,  0.4f, -0.4f,  0.0f, 0.33f, 0.0f,  0.0f, -1.0f,
	 0.4f, -0.4f, -0.4f,  1.0f, 0.66f, 0.0f,  0.0f, -1.0f,
	 0.4f, -0.4f, -0.4f,  1.0f, 0.66f, 0.0f,  0.0f, -1.0f,
	-0.4f,  0.4f, -0.4f,  0.0f, 0.33f, 0.0f,  0.0f, -1.0f,
	 0.4f,  0.4f, -0.4f,  1.0f, 0.33f, 0.0f,  0.0f, -1.0f,
	 //Cara posterior
	 -0.4f, -0.4f,  0.4f,  0.0f, 0.66f, 0.0f,  0.0f, 1.0f,
	  0.4f, -0.4f,  0.4f,  1.0f, 0.66f, 0.0f,  0.0f, 1.0f,
	  0.4f,  0.4f,  0.4f,  1.0f, 0.33f, 0.0f,  0.0f, 1.0f,
	  0.4f,  0.4f,  0.4f,  1.0f, 0.33f, 0.0f,  0.0f, 1.0f,
	 -0.4f,  0.4f,  0.4f,  0.0f, 0.33f, 0.0f,  0.0f, 1.0f,
	 -0.4f, -0.4f,  0.4f,  0.0f, 0.66f, 0.0f,  0.0f, 1.0f,
	 //Cara izquierda
	 -0.4f,  0.4f,  0.4f,  1.0f, 0.33f, -1.0f,  0.0f,  0.0f,
	 -0.4f,  0.4f, -0.4f,  0.0f, 0.33f, -1.0f,  0.0f,  0.0f,
	 -0.4f, -0.4f, -0.4f,  0.0f, 0.66f, -1.0f,  0.0f,  0.0f,
	 -0.4f, -0.4f, -0.4f,  0.0f, 0.66f, -1.0f,  0.0f,  0.0f,
	 -0.4f, -0.4f,  0.4f,  1.0f, 0.66f, -1.0f,  0.0f,  0.0f,
	 -0.4f,  0.4f,  0.4f,  1.0f, 0.33f, -1.0f,  0.0f,  0.0f,
	 //Cara derecha
	  0.4f,  0.4f,  0.4f,  1.0f, 0.33f, 1.0f,  0.0f,  0.0f,
	  0.4f, -0.4f,  0.4f,  1.0f, 0.66f, 1.0f,  0.0f,  0.0f,
	  0.4f, -0.4f, -0.4f,  0.0f, 0.66f, 1.0f,  0.0f,  0.0f,
	  0.4f, -0.4f, -0.4f,  0.0f, 0.66f, 1.0f,  0.0f,  0.0f,
	  0.4f,  0.4f, -0.4f,  0.0f, 0.33f, 1.0f,  0.0f,  0.0f,
	  0.4f,  0.4f,  0.4f,  1.0f, 0.33f, 1.0f,  0.0f,  0.0f,
	  //Cara inferior
	 -0.4f, -0.4f, -0.4f,  0.0f, 1.0f , 0.0f, -1.0f,  0.0f,
	  0.4f, -0.4f, -0.4f,  1.0f, 1.0f , 0.0f, -1.0f,  0.0f,
	  0.4f, -0.4f,  0.4f,  1.0f, 0.66f, 0.0f, -1.0f,  0.0f,
	  0.4f, -0.4f,  0.4f,  1.0f, 0.66f, 0.0f, -1.0f,  0.0f,
	 -0.4f, -0.4f,  0.4f,  0.0f, 0.66f, 0.0f, -1.0f,  0.0f,
	 -0.4f, -0.4f, -0.4f,  0.0f, 1.0f , 0.0f, -1.0f,  0.0f,
	 //Cara superior
	 -0.4f,  0.4f, -0.4f,  0.0f, 0.33f, 0.0f,  1.0f,  0.0f,
	 -0.4f,  0.4f,  0.4f,  0.0f, 0.0f , 0.0f,  1.0f,  0.0f,
	  0.4f,  0.4f,  0.4f,  1.0f, 0.0f , 0.0f,  1.0f,  0.0f,
	  0.4f,  0.4f,  0.4f,  1.0f, 0.0f , 0.0f,  1.0f,  0.0f,
	  0.4f,  0.4f, -0.4f,  1.0f, 0.33f, 0.0f,  1.0f,  0.0f,
	 -0.4f,  0.4f, -0.4f,  0.0f, 0.33f, 0.0f,  1.0f,  0.0f,

	 //Eje
	//Cara frontal
	-1.5f, -0.2f, -0.2f,  0.0f, 0.66f, 0.0f,  0.0f, -1.0f,
	-1.5f,  0.2f, -0.2f,  0.0f, 0.33f, 0.0f,  0.0f, -1.0f,
	 1.5f, -0.2f, -0.2f,  1.0f, 0.66f, 0.0f,  0.0f, -1.0f,
	 1.5f, -0.2f, -0.2f,  1.0f, 0.66f, 0.0f,  0.0f, -1.0f,
	-1.5f,  0.2f, -0.2f,  0.0f, 0.33f, 0.0f,  0.0f, -1.0f,
	 1.5f,  0.2f, -0.2f,  1.0f, 0.33f, 0.0f,  0.0f, -1.0f,
	 //Cara posterior
	 -1.5f, -0.2f,  0.2f,  0.0f, 0.66f, 0.0f,  0.0f, 1.0f,
	  1.5f, -0.2f,  0.2f,  1.0f, 0.66f, 0.0f,  0.0f, 1.0f,
	  1.5f,  0.2f,  0.2f,  1.0f, 0.33f, 0.0f,  0.0f, 1.0f,
	  1.5f,  0.2f,  0.2f,  1.0f, 0.33f, 0.0f,  0.0f, 1.0f,
	 -1.5f,  0.2f,  0.2f,  0.0f, 0.33f, 0.0f,  0.0f, 1.0f,
	 -1.5f, -0.2f,  0.2f,  0.0f, 0.66f, 0.0f,  0.0f, 1.0f,
	 //Cara inferior
	 -1.5f, -0.2f, -0.2f,  0.0f, 1.0f , 0.0f, -1.0f,  0.0f,
	  1.5f, -0.2f, -0.2f,  1.0f, 1.0f , 0.0f, -1.0f,  0.0f,
	  1.5f, -0.2f,  0.2f,  1.0f, 0.66f, 0.0f, -1.0f,  0.0f,
	  1.5f, -0.2f,  0.2f,  1.0f, 0.66f, 0.0f, -1.0f,  0.0f,
	 -1.5f, -0.2f,  0.2f,  0.0f, 0.66f, 0.0f, -1.0f,  0.0f,
	 -1.5f, -0.2f, -0.2f,  0.0f, 1.0f , 0.0f, -1.0f,  0.0f,
	 //Cara superior
	 -1.5f,  0.2f, -0.2f,  0.0f, 0.33f, 0.0f,  1.0f,  0.0f,
	 -1.5f,  0.2f,  0.2f,  0.0f, 0.0f , 0.0f,  1.0f,  0.0f,
	  1.5f,  0.2f,  0.2f,  1.0f, 0.0f , 0.0f,  1.0f,  0.0f,
	  1.5f,  0.2f,  0.2f,  1.0f, 0.0f , 0.0f,  1.0f,  0.0f,
	  1.5f,  0.2f, -0.2f,  1.0f, 0.33f, 0.0f,  1.0f,  0.0f,
	 -1.5f,  0.2f, -0.2f,  0.0f, 0.33f, 0.0f,  1.0f,  0.0f,

	 //brazo1
	 //Cara frontal
	 -1.6f, -0.7f, -0.7f,  0.0f, 0.66f, 0.0f,  0.0f, -1.0f,
	 -1.6f,  0.7f, -0.7f,  0.0f, 0.33f, 0.0f,  0.0f, -1.0f,
	 -1.5f, -0.7f, -0.7f,  1.0f, 0.66f, 0.0f,  0.0f, -1.0f,
	 -1.5f, -0.7f, -0.7f,  1.0f, 0.66f, 0.0f,  0.0f, -1.0f,
	 -1.6f,  0.7f, -0.7f,  0.0f, 0.33f, 0.0f,  0.0f, -1.0f,
	 -1.5f,  0.7f, -0.7f,  1.0f, 0.33f, 0.0f,  0.0f, -1.0f,
	 //Cara posterior
	 -1.6f, -0.7f,  0.7f,  0.0f, 0.66f, 0.0f,  0.0f, 1.0f,
	 -1.5f, -0.7f,  0.7f,  1.0f, 0.66f, 0.0f,  0.0f, 1.0f,
	 -1.5f,  0.7f,  0.7f,  1.0f, 0.33f, 0.0f,  0.0f, 1.0f,
	 -1.5f,  0.7f,  0.7f,  1.0f, 0.33f, 0.0f,  0.0f, 1.0f,
	 -1.6f,  0.7f,  0.7f,  0.0f, 0.33f, 0.0f,  0.0f, 1.0f,
	 -1.6f, -0.7f,  0.7f,  0.0f, 0.66f, 0.0f,  0.0f, 1.0f,
	 //Cara izquierda
	 -1.6f,  0.7f,  0.7f,  1.0f, 0.33f, -1.0f,  0.0f,  0.0f,
	 -1.6f,  0.7f, -0.7f,  0.0f, 0.33f, -1.0f,  0.0f,  0.0f,
	 -1.6f, -0.7f, -0.7f,  0.0f, 0.66f, -1.0f,  0.0f,  0.0f,
	 -1.6f, -0.7f, -0.7f,  0.0f, 0.66f, -1.0f,  0.0f,  0.0f,
	 -1.6f, -0.7f,  0.7f,  1.0f, 0.66f, -1.0f,  0.0f,  0.0f,
	 -1.6f,  0.7f,  0.7f,  1.0f, 0.33f, -1.0f,  0.0f,  0.0f,
	 //Cara derecha
	 -1.5f,  0.7f,  0.7f,  1.0f, 0.33f, 1.0f,  0.0f,  0.0f,
	 -1.5f, -0.7f,  0.7f,  1.0f, 0.66f, 1.0f,  0.0f,  0.0f,
	 -1.5f, -0.7f, -0.7f,  0.0f, 0.66f, 1.0f,  0.0f,  0.0f,
	 -1.5f, -0.7f, -0.7f,  0.0f, 0.66f, 1.0f,  0.0f,  0.0f,
	 -1.5f,  0.7f, -0.7f,  0.0f, 0.33f, 1.0f,  0.0f,  0.0f,
	 -1.5f,  0.7f,  0.7f,  1.0f, 0.33f, 1.0f,  0.0f,  0.0f,
	 //Cara inferior
	-1.6f, -0.7f, -0.7f,  0.0f, 1.0f , 0.0f, -1.0f,  0.0f,
	-1.5f, -0.7f, -0.7f,  1.0f, 1.0f , 0.0f, -1.0f,  0.0f,
	-1.5f, -0.7f,  0.7f,  1.0f, 0.66f, 0.0f, -1.0f,  0.0f,
	-1.5f, -0.7f,  0.7f,  1.0f, 0.66f, 0.0f, -1.0f,  0.0f,
	-1.6f, -0.7f,  0.7f,  0.0f, 0.66f, 0.0f, -1.0f,  0.0f,
	-1.6f, -0.7f, -0.7f,  0.0f, 1.0f , 0.0f, -1.0f,  0.0f,
	//Cara superior
	-1.6f,  0.7f, -0.7f,  0.0f, 0.33f, 0.0f,  1.0f,  0.0f,
	-1.6f,  0.7f,  0.7f,  0.0f, 0.0f , 0.0f,  1.0f,  0.0f,
	-1.5f,  0.7f,  0.7f,  1.0f, 0.0f , 0.0f,  1.0f,  0.0f,
	-1.5f,  0.7f,  0.7f,  1.0f, 0.0f , 0.0f,  1.0f,  0.0f,
	-1.5f,  0.7f, -0.7f,  1.0f, 0.33f, 0.0f,  1.0f,  0.0f,
	-1.6f,  0.7f, -0.7f,  0.0f, 0.33f, 0.0f,  1.0f,  0.0f,

	//brazo2
	//Cara frontal
	1.5f, -0.7f, -0.7f, 0.0f, 0.66f, 0.0f, 0.0f, -1.0f,
	1.5f, 0.7f, -0.7f, 0.0f, 0.33f, 0.0f, 0.0f, -1.0f,
	1.6f, -0.7f, -0.7f, 1.0f, 0.66f, 0.0f, 0.0f, -1.0f,
	1.6f, -0.7f, -0.7f, 1.0f, 0.66f, 0.0f, 0.0f, -1.0f,
	1.5f, 0.7f, -0.7f, 0.0f, 0.33f, 0.0f, 0.0f, -1.0f,
	1.6f, 0.7f, -0.7f, 1.0f, 0.33f, 0.0f, 0.0f, -1.0f,
	//Cara posterior
	1.5f, -0.7f, 0.7f, 0.0f, 0.66f, 0.0f, 0.0f, 1.0f,
	1.6f, -0.7f, 0.7f, 1.0f, 0.66f, 0.0f, 0.0f, 1.0f,
	1.6f, 0.7f, 0.7f, 1.0f, 0.33f, 0.0f, 0.0f, 1.0f,
	1.6f, 0.7f, 0.7f, 1.0f, 0.33f, 0.0f, 0.0f, 1.0f,
	1.5f, 0.7f, 0.7f, 0.0f, 0.33f, 0.0f, 0.0f, 1.0f,
	1.5f, -0.7f, 0.7f, 0.0f, 0.66f, 0.0f, 0.0f, 1.0f,
	//Cara izquierda
	1.5f, 0.7f, 0.7f, 1.0f, 0.33f, -1.0f, 0.0f, 0.0f,
	1.5f, 0.7f, -0.7f, 0.0f, 0.33f, -1.0f, 0.0f, 0.0f,
	1.5f, -0.7f, -0.7f, 0.0f, 0.66f, -1.0f, 0.0f, 0.0f,
	1.5f, -0.7f, -0.7f, 0.0f, 0.66f, -1.0f, 0.0f, 0.0f,
	1.5f, -0.7f, 0.7f, 1.0f, 0.66f, -1.0f, 0.0f, 0.0f,
	1.5f, 0.7f, 0.7f, 1.0f, 0.33f, -1.0f, 0.0f, 0.0f,
	//Cara derecha
	1.6f, 0.7f, 0.7f, 1.0f, 0.33f, 1.0f, 0.0f, 0.0f,
	1.6f, -0.7f, 0.7f, 1.0f, 0.66f, 1.0f, 0.0f, 0.0f,
	1.6f, -0.7f, -0.7f, 0.0f, 0.66f, 1.0f, 0.0f, 0.0f,
	1.6f, -0.7f, -0.7f, 0.0f, 0.66f, 1.0f, 0.0f, 0.0f,
	1.6f, 0.7f, -0.7f, 0.0f, 0.33f, 1.0f, 0.0f, 0.0f,
	1.6f, 0.7f, 0.7f, 1.0f, 0.33f, 1.0f, 0.0f, 0.0f,
	//Cara inferior
	1.5f, -0.7f, -0.7f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
	1.6f, -0.7f, -0.7f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
	1.6f, -0.7f, 0.7f, 1.0f, 0.66f, 0.0f, -1.0f, 0.0f,
	1.6f, -0.7f, 0.7f, 1.0f, 0.66f, 0.0f, -1.0f, 0.0f,
	1.5f, -0.7f, 0.7f, 0.0f, 0.66f, 0.0f, -1.0f, 0.0f,
	1.5f, -0.7f, -0.7f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
	//Cara superior
	1.5f, 0.7f, -0.7f, 0.0f, 0.33f, 0.0f, 1.0f, 0.0f,
	1.5f, 0.7f, 0.7f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	1.6f, 0.7f, 0.7f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	1.6f, 0.7f, 0.7f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	1.6f, 0.7f, -0.7f, 1.0f, 0.33f, 0.0f, 1.0f, 0.0f,
	1.5f, 0.7f, -0.7f, 0.0f, 0.33f, 0.0f, 1.0f, 0.0f
};