


#include "libft.h"
#include "doomnukem.h"
#include "objects.h"

void	printcolor(uint32_t clr)
{
	printf("R: %i G: %i B: %i \n", clr & 0xFF, (clr >> 8) & 0xFF, (clr >> 16) & 0xFF);
}

t_material	parsemat(int fd, char *name)
{
	t_material	mat;
	char		*line;
	char		**kd_strs;
	int			i;

	ft_bzero(&mat, sizeof(t_material));
	ft_strcpy(mat.name, name);
	while (ft_get_next_line(fd, &line))
	{
		if (ft_strnstr(line, "Kd ", 3) != NULL)
		{
			#ifdef	OBJ_DEBUG
			printf("	found Kd for '%s' \n", mat.name);
			#endif
			kd_strs = ft_strsplit(line + sizeof("Kd"), ' ');
			i = 0;
			while (i < 3 && kd_strs[i] != NULL)
			{
				mat.kd += ((int)(atof(kd_strs[i]) * 255.0f) & 0xFF) << (8 * i);
				free(kd_strs[i]);
				i++;
			}
			free(kd_strs);
			if (i != 3)
				printf("invalid kd string! \n"); //TODO: log?
		}
		if (ft_strlen(line) == 0)
		{
			free(line);
			break ;
		}
		free (line);
	}
	return (mat);
}

void	parse_mtllib(t_list **list, char *filename)
{
	char		*line;
	int			fd;
	t_material	mat;
	char		fullname[256];

	sprintf(fullname, "%s%s", OBJPATH, filename);
	fd = open(fullname, O_RDONLY);//TODO: concat assetpath before filename
	if (fd == -1)
		return ;
	while (ft_get_next_line(fd, &line))
	{
		if (ft_strstr(line, "newmtl ") != NULL)
		{
			mat = parsemat(fd, line + sizeof("newmtl"));
			list_push(list, &mat, sizeof(t_material));
			#ifdef	OBJ_DEBUG
			printf("	 ");
			printcolor(mat.kd);
			printf("\n");
			#endif
		}
		free(line);
	}
}

t_list	*get_material_list(int fd)
{
	char	*line;
	t_list	*list;

	list = NULL;
	while (ft_get_next_line(fd, &line))
	{
		if (ft_strnstr(line, "mtllib ", sizeof("mtllib")) != NULL)
			parse_mtllib(&list, line + sizeof("mtllib"));
		free(line);
	}
	lseek(fd, 0, SEEK_SET);
	return (list);
}

void	free_list_node(void *content, size_t size)
{
	free(content);
}

t_object	objparse(char *filename)
{
	t_object	result;
	t_list		*materials;
	t_list		*vertices;
	t_list		*uvs;
	t_list		*faces;
	int			fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return ((t_object){"NULLOBJECT"}); //TODO: return a crash bandicoot here
	ft_bzero(&result, sizeof(t_object));
	
	printf("reading object %s \n", filename);
	materials = get_material_list(fd);
	vertices = get_vertex_list(fd);
	uvs = get_uv_list(fd);
	faces = get_face_list(fd);
	#ifdef	OBJ_DEBUG
	printf("found %i materials \n", ft_listlen(materials));
	printf("found %i vertices \n", ft_listlen(vertices));
	printf("found %i uvs \n", ft_listlen(uvs));
	printf("found %i faces (triangles) \n", ft_listlen(faces));
	#endif
	result.materials = list_to_ptr(materials, &result.material_count);
	result.vertices = list_to_ptr(vertices, &result.vertice_count);
	result.uvs = list_to_ptr(uvs, &result.uv_count);
	result.faces = list_to_ptr(faces, &result.face_count);

	ft_lstdel(&materials, free_list_node);
	ft_lstdel(&vertices, free_list_node);
	ft_lstdel(&uvs, free_list_node);
	ft_lstdel(&faces, free_list_node);
	#ifdef	OBJ_DEBUG
	printf("converted %i materials to pointer format\n", result.material_count); //TODO: log
	printf("converted %i vertices to pointer format\n", result.vertice_count); //TODO: log
	printf("converted %i uvs to pointer format\n", result.uv_count);
	printf("converted %i faces to pointer format\n", result.face_count); //TODO: log
	#endif
	return (result);
}