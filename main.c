#include "fractol.h"

void	draw_set(t_vars *vars)
{
	int				x;
	int				y;
	t_complex		complex;

	y = 0;
	while (y < WIN_SIZE)
	{
		x = 0;
		while (x < WIN_SIZE)
		{
			complex.b = map(y, WIN_SIZE, vars->zoom->ymin, vars->zoom->ymax);
			complex.a = map(x, WIN_SIZE, vars->zoom->xmin, vars->zoom->xmax);
			complex.az = complex.a;
			complex.bz = complex.b;
			if (vars->type)
			{
				complex.az = vars->xconst;
				complex.bz = vars->yconst;
			}
			img_mlx_pixel_put(vars->i, x, y, vars->func(&complex, \
			vars->iters, vars->palette));
			x++;
		}
		y++;
	}
}

void	check_args(t_vars *v, char **argv, int argc)
{
	v->type = 0;
	v->zoom = 0;
	v->iters = 0;
	v->i->addr = mlx_get_data_addr(v->i->img, &v->i->bpp, &v->i->l_l, &v->i->e);
	v->img_buff->addr = mlx_get_data_addr(v->img_buff->img, \
	&v->img_buff->bpp, &v->img_buff->l_l, &v->img_buff->e);
	if (argc < 3)
		win_close(v, HELP_MSG);
	v->iters = ft_atof(argv[2]);
	if (argv[1][0] == 'm')
		v->func = calc_mandel;
	else if (argv[1][0] == 'j')
	{
		if (argc < 5)
			win_close(v, HELP_MSG);
		v->xconst = ft_atof(argv[2]);
		v->yconst = ft_atof(argv[3]);
		v->iters = ft_atof(argv[4]);
		v->func = calc_mandel;
		v->type = 1;
	}
	else if (argv[1][0] == 'b')
		v->func = calc_burning_ship;
	else
		win_close(v, ERR_MSG);
}

int	loop(t_vars *vars)
{
	t_data	*temp;

	mlx_clear_window(vars->mlx, vars->mlx_win);
	temp = vars->img_buff;
	vars->img_buff = vars->i;
	vars->i = temp;
	draw_set(vars);
	mlx_put_image_to_window(vars->mlx, vars->mlx_win, vars->i->img, 0, 0);
	return (0);
}

int	main(int argc, char **argv)
{
	t_vars			vars;
	t_data			img;
	t_data			img_b;
	unsigned long	palette[P_SIZE];

	vars.mlx = mlx_init();
	vars.mlx_win = mlx_new_window(vars.mlx, WIN_SIZE, WIN_SIZE, "fract.ol");
	vars.i = &img;
	vars.img_buff = &img_b;
	img.img = mlx_new_image(vars.mlx, WIN_SIZE, WIN_SIZE);
	img_b.img = mlx_new_image(vars.mlx, WIN_SIZE, WIN_SIZE);
	if (!img.img || !img_b.img)
		win_close(&vars, ERR_MSG);
	if (COL_SET)
		generate_palette((unsigned long *)&palette);
	vars.palette = (unsigned long *)&palette;
	check_args(&vars, argv, argc);
	set_zoom(&vars, new_coords(-2, 2, -2, 2));
	mlx_mouse_hook(vars.mlx_win, zoom, &vars);
	mlx_key_hook(vars.mlx_win, key, &vars);
	mlx_hook(vars.mlx_win, 17, 0, win_close, &vars);
	mlx_loop_hook(vars.mlx, loop, &vars);
	mlx_loop(vars.mlx);
}
