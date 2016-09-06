OffModel *protein;

static void  CreateBuffers(char* OffFile)
{
	float maxBoundX=0,maxBoundY=0,maxBoundZ=0,minBoundX=0,minBoundY=0,minBoundZ=0;
	float x,y,z;
	float BoundCenter[3],Scale;
	float dX,dY,dZ,BoundBoxWidth;
	int i,j;
	protein=readOffFile(OffFile);


	for (i = 0; i < protein->numberOfVertices; i++)
	 {
	 	x=(protein->vertices[i]).x;
	 	y=(protein->vertices[i]).y;
	 	z=(protein->vertices[i]).z;
	 	if(i==0)
	 	{
	 		maxBoundX=x;maxBoundY=y;maxBoundZ=z;
	 		minBoundX=x;minBoundY=y;minBoundZ=z;
	 	}
	 	else
	 	{
	 		if(x>maxBoundX)
	 			maxBoundX=x;
	 		else if(x<minBoundX)
	 			minBoundX=x;

	 		if(y>maxBoundY)
	 			maxBoundY=y;
	 		else if(y<minBoundY)
	 			minBoundY=y;

	 		if(z>maxBoundZ)
	 			maxBoundZ=z;
	 		else if(z<minBoundZ)
	 			minBoundZ=z; 
	 	}
	 }
	dX=maxBoundX- minBoundX;
	dY=maxBoundY- minBoundY;
	dZ=maxBoundZ- minBoundZ;
	
	if(dX>dY)
	{
		if(dX>dZ)
			BoundBoxWidth=dX;
		else
			BoundBoxWidth=dZ;
	}
	else
	{
		if(dY>dZ)
			BoundBoxWidth=dY;
		else
			BoundBoxWidth=dZ;
	}
	if(BoundBoxWidth==0)
		BoundBoxWidth=1;
	Scale = 1.0/BoundBoxWidth; 
	BoundCenter[0]=Scale*(maxBoundX-(dX/2));
	BoundCenter[1]=Scale*(maxBoundY-(dY/2));
	BoundCenter[2]=Scale*(maxBoundZ-(dZ/2));
	for (i = 0; i < protein->numberOfVertices; i++)
	 {
	 	(protein->vertices[i]).x*=Scale;
	 	(protein->vertices[i]).y*=Scale;
	 	(protein->vertices[i]).z*=Scale;
	 	(protein->vertices[i]).x-=BoundCenter[0];
	 	(protein->vertices[i]).y-=BoundCenter[1];
	 	(protein->vertices[i]).z-=BoundCenter[2];
	 }
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*(protein->numberOfVertices), protein->vertices, GL_STATIC_DRAW);

	int* Indices = NULL;
	Indices=new int[protein->numberOfTriangles*3];
	if (Indices==NULL){
		cout << "Error: Memory couldnot be allocated"<< endl;
	 	exit(1);
	}
	for (i=0,j=0;i < protein->numberOfTriangles; i++,j+=3)
	{
	 	Indices[j]=(protein->triangles[i]).v[0];
	 	Indices[j+1]=(protein->triangles[i]).v[1];
	 	Indices[j+2]=(protein->triangles[i]).v[2];
	}
	glGenBuffers(1,&IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*(protein->numberOfTriangles)*3, Indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}