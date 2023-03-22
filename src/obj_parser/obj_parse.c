
#include "libft.h"
#include "doomnukem.h"
#include "objects.h"

t_material	parsemat(int fd, char *name)
{
	t_material	mat;
	char		*line;
	char		**kd_strs;
	int			i;
	int			ret;

	ft_bzero(&mat, sizeof(t_material));
	ft_strcpy(mat.name, name);
	line = NULL;
	ret = get_next_line(fd, &line);
	while (ret)
	{
		if (line)
		{
			if (ft_strnstr(line, "Kd ", 3) != NULL)
			{
				kd_strs = ft_strsplit(line + sizeof("Kd"), ' ');
				if (kd_strs == NULL)
					doomlog(LOG_EC_MALLOC, "parsemat");
				i = 0;
				while (i < 3 && kd_strs[i] != NULL)
				{
					// TODO: make own version of atof()
					mat.kd += ((int)(atof(kd_strs[i]) * 255.0f) & 0xFF) << (8 * i);
					free(kd_strs[i]);
					kd_strs[i] = NULL;
					i++;
				}
				free(kd_strs);
				kd_strs = NULL;
				if (i != 3)
					doomlog(LOG_FATAL, "invalid kd string!");
			}
			if (ft_strnstr(line, "map_Kd ", sizeof("map_Kd")) != NULL)
			{
				if (ft_strrchr(line, '/') != NULL)
					ft_strcpy(mat.texturename, ft_strrchr(line, '/') + 1);
				else
					ft_strcpy(mat.texturename, line + sizeof("map_Kd"));
			}
			free(line);
			line = NULL;
		}
		ret = get_next_line(fd, &line);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "parsemat");
	return (mat);
}

void	parse_mtllib(t_list **list, char *filename)
{
	char		*line;
	int			fd;
	t_material	mat;
	char		mat_path[256];
	int			ret;

	ft_strcpy(mat_path, OBJPATH);
	ft_strcat(mat_path, filename);
	fd = open(mat_path, O_RDONLY);
	
	if (fd == -1)
		return ;
	line = NULL;
	ret = get_next_line(fd, &line);
	while (ret)
	{
		if (line)
		{
			if (ft_strstr(line, "newmtl ") != NULL)
			{
				mat = parsemat(fd, line + sizeof("newmtl"));
				list_push(list, &mat, sizeof(t_material));
			}
			free(line);
			line = NULL;
		}
		ret = get_next_line(fd, &line);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, mat_path);
	if (close(fd) == -1)
		doomlog(LOG_EC_CLOSE, mat_path);
}

t_list	*get_material_list(int fd)
{
	char	*line;
	t_list	*list;
	int		ret;

	list = NULL;
	line = NULL;
	ret = get_next_line(fd, &line);
	while (ret)
	{
		if (line)
		{
			if (ft_strnstr(line, "mtllib ", sizeof("mtllib")) != NULL)
				parse_mtllib(&list, line + sizeof("mtllib"));
			free(line);
			line = NULL;
		}
		
		ret = get_next_line(fd, &line);
	}
	if (ret == -1)
		doomlog(LOG_EC_GETNEXTLINE, "get_material_list");
	if (lseek(fd, 0, SEEK_SET) == -1)
		doomlog(LOG_EC_LSEEK, "get_material_list");
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
	materials = get_material_list(fd);
	vertices = get_vertex_list(fd);
	uvs = get_uv_list(fd);
	faces = get_face_list(fd, materials);
	result.materials = list_to_ptr(materials, &result.material_count);
	if (result.materials == NULL)
		result.materials = ft_memalloc(sizeof(t_material));
	result.vertices = list_to_ptr(vertices, &result.vertice_count);
	result.uvs = list_to_ptr(uvs, &result.uv_count);
	result.faces = list_to_ptr(faces, &result.face_count);
	ft_lstdel(&materials, free_list_node);
	ft_lstdel(&vertices, free_list_node);
	ft_lstdel(&uvs, free_list_node);
	ft_lstdel(&faces, free_list_node);
	return (result);
}