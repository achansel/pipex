/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strncat_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansel <achansel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 00:01:11 by achansel          #+#    #+#             */
/*   Updated: 2022/03/28 00:08:33 by achansel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strncat(char *dest, char *src, int sz)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dest[i])
		i++;
	while (src[j] && i + j < sz)
	{
		dest[i + j] = src[j];
		j++;
	}
	if (i + j < sz)
		dest[i + j] = '\0';
	else
		dest[i + j - 1] = '\0';
	return (dest);
}
