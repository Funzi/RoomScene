const int floor_vertices_count = 4;
const float floor_vertices[floor_vertices_count * 8] =
{
	//       Position                 Normal             Tex Coord
	
	// Bottom face
	-1.0f,-1.0f, 1.0f,		 0.0f,1.0f, 0.0f,		0.0f, 2.0f,
	-1.0f,-1.0f,-1.0f,		 0.0f,1.0f, 0.0f,		0.0f, 0.0f,
	1.0f,-1.0f,-1.0f,		 0.0f,1.0f, 0.0f,		2.0f, 0.0f,
	1.0f,-1.0f, 1.0f,		 0.0f,1.0f, 0.0f,		2.0f, 2.0f,
};

const int floor_indices_count = 30;
const unsigned int floor_indices[floor_indices_count] =
{
	0, 1, 2,		0, 2, 3,		// Bottom face
};
