const int map_vertices_count = 4;
const float map_vertices[map_vertices_count * 8] =
{
	//       Position                 Normal             Tex Coord
	
	// Right face
	1.0f, 1.0f,-1.0f,		 0.0f, 0.0f,-1.0f,		 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,		 0.0f, 0.0f,-1.0f,		 1.0f, 0.0f,
	-1.0f,-1.0f,-1.0f,		 0.0f, 0.0f,-1.0f,		 0.0f, 0.0f,
	-1.0f, 1.0f,-1.0f,		 0.0f, 0.0f,-1.0f,		 0.0f, 1.0f,
	// Right face
	
};

const int map_indices_count = 6;
const unsigned int map_indices[map_indices_count] =
{
	
	0, 1, 2,		0, 2, 3,		// Right face
	
};
