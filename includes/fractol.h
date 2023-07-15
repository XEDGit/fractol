/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fractol.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lmuzio <lmuzio@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/18 18:01:05 by lmuzio        #+#    #+#                 */
/*   Updated: 2023/07/09 13:24:30 by lmuzio        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "MLX42/MLX42_Int.h"
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <pthread.h>

# define HELP_MSG "This is a help message, to run the program \
without displaying this do not add arguments after ./fractol\n\n\
Commands:\n\
\t[TAB]:		Change fractal displayed\n\
\t[SPACE]:	Auto zoom\n\
\t[-] [+](=):	Zoom in/out\n\
\tSCROLL:		Zoom in/out\n\
\t[J]:		Visualize Julia's set of the current fractal\n\
\tRCLICK:		Visualize Julia's set at mouse coordinates\n\
\t[W][A][S][D]:	Change constants of Julia's set\n\
\t[R]:		Reload initial state\n\
\t[C]:		Change color set\n\
\t[.] [,]:	Shift color\n\
\t[/]:		Reset color shift\n\
\t[;] [']:	Multiply color\n\
\t[\\]:		Reset color multiply\n\
\t[ARROWS]:	Move within the scene\n\
\t[ESC]:		Exit\n"
# define ERR_MSG "Program terminated. An error occourred\n"
# define P_SIZE 16
# define MAX_THREADS 50
# define AUTOZOOM_FRAMES 10

typedef struct s_viewport_coordinates
{
	long double	xmin;
	long double	xmax;
	long double	ymin;
	long double	ymax;
}	t_coords;

typedef struct s_complex
{
	long double	a;
	long double	az;
	long double	b;
	long double	bz;
}	t_complex;

typedef struct s_instance_variables	t_vars;

typedef size_t						(*t_func)(t_complex*, int, t_vars *);

typedef struct s_instance_variables
{
	mlx_t			*mlx;
	int				type;
	t_func			func;
	t_coords		zoom;
	mlx_image_t		*i;
	unsigned long	palette[P_SIZE + 1];
	long double		xconst;
	long double		yconst;
	int				iters;
	int				x_res;
	int				y_res;
	long double		j_step;
	int				color_set;
	long double		multiply;
	int				typeog;
	int				autozoom;
	int				update;
}	t_vars;

typedef struct s_thread_variables
{
	pthread_t	thread;
	t_complex	complex;
	t_vars		*vars;
	int			x;
	int			y;
	int			y_fract;
	mlx_image_t	*i;
	t_coords	zoom;
}	t_threadvars;

typedef struct s_autozoom_frame
{
	t_coords	zoom;
	mlx_image_t	*i;
}	t_azframe;

enum e_fractal
{
	MANDELBROT,
	BURNING_SHIP,
	NUM_FRACTALS
};

void		draw_set(t_vars *vars);
void		*thread_main(void *vvars);
size_t		modulo_colors(long double z, int n, int iter, t_vars *vars);
void		generate_palette(unsigned long *palette);
int			win_close(t_vars *vars, char *msg);
void		key(mlx_key_data_t keydata, void *vvars);
void		zoom(double xstep, double ystep, void *vvars);
void		resize(int width, int height, void *vvars);
void		mouse(mouse_key_t button, action_t action, \
				modifier_key_t mods, void *vvars);
void		print(const char *s);
void		change_fractal(t_vars *vars, int type);
void		shift_zoom(t_vars *vars, int x, int y, int direction);
int			shift_view(int key, t_vars *vars, long double xstep, \
					long double ystep);
size_t		calc_mandel(t_complex *comp, int iter, t_vars *vars);
size_t		calc_burning_ship(t_complex *comp, int iter, t_vars *vars);
long double	map(long double middle, long double win_size, \
					long double min, long double max);
void		draw_autozoom_set(t_vars *vars, t_azframe *imgs);
void		free_imgs(t_vars *vars, t_azframe *imgs);
t_azframe	*gen_autozoom(t_vars *vars);
void		handle_autozoom(t_vars *vars);

#endif