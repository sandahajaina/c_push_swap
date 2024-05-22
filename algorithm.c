/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:51:28 by sranaivo          #+#    #+#             */
/*   Updated: 2024/05/13 11:51:48 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/* retourne le rank de *to_find */
int	find_rank(t_stack **stack, t_stack *to_find)
{
	int		i;
	t_stack	*temp;

	temp = *stack;
	i = 0;
	while (temp)
	{
		i++;
		if (to_find == temp)
			break ;
		temp = temp->next;
	}
	return (i);
}

// cherche le target node de *to_find (from a to b)
t_stack	*find_target(t_stack **stack, t_stack *to_find)
{
	int		n;
	t_stack	*prev;
	t_stack	*temp;

	temp = *stack;
	prev = stack_min(*stack);
	n = to_find->n;
	if (n < prev->n)
		return (stack_max(*stack));
	while (temp)
	{
		if ((temp->n < n) && (temp->n > prev->n))
			prev = temp;
		temp = temp->next;
	}
	return (prev);
}

// compte le nb de move pour palcer *to_find au top du stack
int	count_move(t_stack *stack, t_stack *to_find)
{
	int	move;
	int	median;
	int	index;
	int	nb;

	move = 0;
	nb = count_node(stack);
	median = nb / 2;
	index = get_index(stack, to_find);
	if (index <= median)
		move = index;
	else
		move = nb - index;
	return (move);
}

static	int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

int count_total_move(t_stack **a, t_stack **b, t_stack *node, t_stack *target)
{
	int	index_a;
	int	index_b;
	int	median_a;
	int	median_b;
	int	move;

	index_a = find_rank(a, node);
	index_b = find_rank(b, target);
	median_a = count_node(*a) / 2;
	median_b = count_node(*b) / 2;
	if(index_a <= median_a && index_b <= median_b)
		move = max(count_move(*a, node), count_move(*b, target));
	else if (index_a > median_a && index_b > median_b)
		move = max(count_move(*a, node), count_move(*b, target));
	else
		move = count_move(*a, node) + count_move(*b, target);
	return (move);
}

// recherche le champion
t_stack	*find_cheapest(t_stack **a, t_stack **b)
{
	int		move;
	int		min;
	t_stack	*tmp;
	t_stack	*target;
	t_stack	*champion;

	tmp = *a;
	min = count_node(*a) / 2;
	champion = *a;
	while (tmp)
	{
		target = find_target(b, tmp);
		move = count_total_move(a, b, tmp, target);
		if (move < min)
		{
			min = move;
			champion = tmp;
		}
		tmp = tmp->next;
	}
	return (champion);
}

// get the index
int	get_index(t_stack *stack, t_stack *to_find)
{
	int		i;
	t_stack	*temp;

	temp = stack;
	i = 0;
	while (temp != to_find)
	{
		i++;
		temp = temp->next;
	}
	return (i);
}

static t_stack	*get_the_node(t_stack **a, t_stack **b, t_stack *x, t_stack *y)
{
	if (find_rank(a, x) <= find_rank(b, y))
		return (x);
	return (y);
}

int	is_there(t_stack **a, t_stack *node)
{
	t_stack	*tmp;

	tmp = *a;
	while (tmp)
	{
		if (node == tmp)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

static void	rr_before_push(t_stack **a, t_stack **b, t_stack *min)
{
	t_stack	*target;
	t_stack	*the_node;
	int		median_a;
	int		median_b;

	target = find_target(b, min);
	median_a = count_node(*a) / 2;
	median_b = count_node(*b) / 2;
	if (find_rank(a, min) <= median_a && find_rank(b, target) <= median_b)
	{
		the_node = get_the_node(a, b, min, target);
		if (is_there(a, the_node))
			while (*a != the_node)
				rr(a, b);
		else
			while (*b != the_node)
				rr(a, b);
	}
	return ;
}

static void	rrr_before_push(t_stack **a, t_stack **b, t_stack *min)
{
	t_stack	*target;
	t_stack	*the_node;
	int		median_a;
	int		median_b;

	target = find_target(b, min);
	median_a = count_node(*a) / 2;
	median_b = count_node(*b) / 2;
	if (find_rank(a, min) > median_a && find_rank(b, target) > median_b)
	{
		the_node = get_the_node(a, b, min, target);
		if (is_there(a, the_node))
			while (*a != the_node)
				rrr(a, b);
		else
			while (*b != the_node)
				rrr(a, b);
	}
	return ;
}

//push to b 'till 3
void	push_to_b(t_stack **a, t_stack **b)
{
	t_stack	*min;
	t_stack	*target;
	int		i;

	i = count_node(*a);
	while(i > 3)
	{
		min = find_cheapest(a, b);
		rr_before_push(a, b, min);
		rrr_before_push(a, b, min);
		if ((find_rank(a, min)) <= (count_node(*a) / 2))
			while(*a != min)
				ra(a);
		else
			while(*a != min)
				rra(a);
		target = find_target(b, min);
		if ((find_rank(b, target)) <= (count_node(*b) / 2))
			while(*b != target)
				rb(b);
		else
			while(*b != target)
				rrb(b);
		pb(a, b);
		i--;
	}
}

t_stack	*find_position(t_stack **stack, t_stack *to_find)
{
	int		n;
	t_stack	*next;
	t_stack	*temp;

	temp = *stack;
	next = stack_max(*stack);
	n = to_find->n;
	if (n > next->n)
		return (stack_min(*stack));
	while (temp)
	{
		if ((temp->n > n) && (temp->n < next->n))
			next = temp;
		temp = temp->next;
	}
	return (next);
}

void	push_to_a(t_stack **a, t_stack **b)
{
	t_stack	*target;
	t_stack	*min;

	while(*b)
	{
		target = find_position(a, *b);
		if (find_rank(a, target) <= (count_node(*a) / 2))
			while(*a != target)
				ra(a);
		else
			while(*a != target)
				rra(a);
		pa(b, a);
	}
	min = stack_min(*a);
	if ((find_rank(a, min) + 1) <= (count_node(*a) / 2))
		while(*a != min)
			ra(a);
	else
		while(*a != min)
			rra(a);
}

void	sort_stack(t_stack **a, t_stack **b)
{
	if (count_node(*a) == 2)
	{
		if (!check_sorted(a))
			sa(a);
		return ;
	}
	else if (count_node(*a) == 3)
	{
		sort_three(a);
		return ;
	}
	else if (count_node(*a) == 4)
		pb(a, b);
	else
	{
		pb(a, b);
		pb(a, b);
	}
	push_to_b(a, b);
	sort_three(a);
	push_to_a(a, b);
}
