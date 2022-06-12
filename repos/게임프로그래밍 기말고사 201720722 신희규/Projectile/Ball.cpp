
#include "Ball.h"
#include "SDL_image.h"
#include <iostream>

Ball::Ball(double radius, Room *room)
{
	room_ = room;
	radius_ = radius;

	v_[0] = 0;
	v_[1] = 0;

	mass_ = 2; // 2kg
	coeff_of_restitution_ = 0.7;
	coeff_of_friction_ = 0.7f;
	
	Reset();
}

void
Ball::Reset()
{
	prev_p_[0] = 0;
	prev_p_[1] = 0;

	p_[0] = radius_ + room_->left_wall_x();
	p_[1] = radius_;

	v_[0] = 0;
	v_[1] = 0;
}

void
Ball::Launch(double initial_force_x, double initial_force_y)
{
	v_[0] = v_[0] + (initial_force_x/mass_);
	v_[1] = v_[1] + (initial_force_y/mass_);
}

void 
Ball::Update(double timestep_s)
{

	double dt = timestep_s;	// seconds
	
	// ���ӵ�
	double a[2];
	a[0] = 0;
	a[1] = room_->gravitational_acc_y();// Gravity

	// Move, p[0] = ���� �߽� x��ǥ, p[1] = ���� �߽� y��ǥ
	prev_p_[0] = p_[0];
	prev_p_[1] = p_[1];

	p_[0] = p_[0] + dt * v_[0];
	p_[1] = p_[1] + dt * v_[1];


	// Collision with Ground
	if ( p_[1]-radius_ < room_->ground_height() && v_[1] < 0 ) 
	{
		p_[1] = radius_ + room_->ground_height();

		v_[1] = -1*v_[1];

		// Coefficient of friction
		v_[0] = coeff_of_friction_ * v_[0];

		// Coefficient of restitution
		v_[1] = coeff_of_restitution_ * v_[1];
	}
	// Collision with Ceilling
	else if ( p_[1]+radius_ > room_->height() && v_[1] > 0 ) 
	{
		p_[1] = room_->height() - radius_;

		v_[1] = -1*v_[1];

		// Coefficient of restitution
		v_[1] = coeff_of_restitution_ * v_[1];
	}

	// Collision with Left Wall
	if ( p_[0] - radius_ < room_->left_wall_x() && v_[0] < 0 )
	{
		p_[0] = room_->left_wall_x() + radius_;

		v_[0] = -1 * v_[0];		

		// Coefficient of restitution
		v_[0] = coeff_of_restitution_ * v_[0];
	}

	// Collision with Right Wall
	else if (p_[0] + radius_ > room_->right_wall_x() && v_[0] > 0)
	{
		p_[0] = room_->right_wall_x() - radius_;

		v_[0] = -1 * v_[0];

		// Coefficient of restitution
		v_[0] = coeff_of_restitution_ * v_[0];
	}

	double fenec_edge_x = room_->vertiacal_fence_pos_x();
	double fenec_edge_y = room_->vertiacal_fence_height();

	double dist = (fenec_edge_x - p_[0]) * (fenec_edge_x - p_[0]) +
		(fenec_edge_y - p_[1]) * (fenec_edge_y - p_[1]);

	// ���� ���� ���� ���� ���� ������ ���� ���
	double grad = (prev_p_[1] - p_[1]) / (prev_p_[0] - p_[0]);

	// ����� ����, �־��� �� �� �� �� ���� �������� y = grad * x + b�� b�� ����
	double b = p_[1] - grad * p_[0];
	 
	// Collision with Fence edge
	if (dist < radius_ * radius_
		&& p_[0] > room_->vertiacal_fence_pos_x() && v_[0] > 0 && v_[1] < 0)
	{
		std::cout << "Edge, Go Right\n";

		// ������ �������� ���� x ��ǥ�� ����
		double x = ((room_->vertiacal_fence_height() + radius_) - b) / grad;

		// ��ǥ�� ����
		p_[0] = x;
		p_[1] = room_->vertiacal_fence_height() + radius_;

		v_[1] = -v_[1];
		// Coefficient of restitution
		v_[1] = coeff_of_restitution_ * v_[1];
	}
	else if (dist < radius_ * radius_
		&& p_[0] < room_->vertiacal_fence_pos_x() && v_[0] > 0 && v_[1] < 0)
	{
		std::cout << "Edge, Go Left\n";

		// ������ �������� ���� x ��ǥ�� ����
		double x = ((room_->vertiacal_fence_height() + radius_) - b) / grad;

		// ��ǥ�� ����
		p_[0] = x;
		p_[1] = room_->vertiacal_fence_height() + radius_;

		v_[0] = -v_[0];
		v_[1] = -v_[1];
		
		v_[1] = coeff_of_restitution_ * v_[1];
	}
	else if (dist < radius_ * radius_
		&& p_[0] < room_->vertiacal_fence_pos_x() && v_[0] < 0 && v_[1] < 0)
	{
		std::cout << "Edge, Go Left\n";

		// ������ �������� ���� x ��ǥ�� ����
		double x = ((room_->vertiacal_fence_height() + radius_) - b) / grad;

		// ��ǥ�� ����
		p_[0] = x;
		p_[1] = room_->vertiacal_fence_height() + radius_;

		v_[1] = -v_[1];
		
		v_[1] = coeff_of_restitution_ * v_[1];
	}
	else if (dist < radius_ * radius_
		&& p_[0] > room_->vertiacal_fence_pos_x() && v_[0] < 0 && v_[1] < 0)
	{
		std::cout << "Edge, Go Right\n";

		// ������ �������� ���� x ��ǥ�� ����
		double x = ((room_->vertiacal_fence_height() + radius_) - b) / grad;

		// ��ǥ�� ����
		p_[0] = x;
		p_[1] = room_->vertiacal_fence_height() + radius_;

		v_[0] = -v_[0];
		v_[1] = -v_[1];
		
		v_[1] = coeff_of_restitution_ * v_[1];
	}
	// Collision with Fence of Leftside
	else if (p_[0] + radius_ > room_->vertiacal_fence_pos_x() && p_[0] - radius_*0.5f < room_->vertiacal_fence_pos_x()
		&& p_[1] - radius_ < room_->vertiacal_fence_height() && v_[0] > 0)
	{		
		std::cout << "Collision with Fence of Leftside\n";

		v_[0] = -1 * v_[0];

		// ������ ������
		double y = grad * -radius_ + b;

		// ��ǥ�� ����
		p_[0] = -radius_;
		p_[1] = y;
		
		v_[0] = coeff_of_restitution_ * v_[0];
	}
	// Collision with Fence of Rightside
	else if (p_[0] - radius_ < room_->vertiacal_fence_pos_x() && p_[0] + radius_*0.5f > room_->vertiacal_fence_pos_x()
		&& p_[1] - radius_ < room_->vertiacal_fence_height() && v_[0] < 0)
	{
		std::cout << "Collision with Fence of Rightside\n";
		v_[0] = -1 * v_[0];
		
		// ������ ������
		double y = grad * radius_ + b;

		// ��ǥ�� ����
		p_[0] = radius_;
		p_[1] = y;
		
		v_[0] = coeff_of_restitution_ * v_[0];
	}

	//�ӵ� = �����ӵ� + �ð�(dt) * ���ӵ�;
	v_[0] = v_[0] + dt * a[0];
	v_[1] = v_[1] + dt * a[1];
}
