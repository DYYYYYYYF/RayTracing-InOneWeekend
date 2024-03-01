#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "hittable.h"

struct hit_record;

class material {
public:

	//��ɢ����
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

//����ɢ��
class lambertian : public material {
public:
	lambertian() : albedo(color(1.0, 1.0, 1.0)) {}
	lambertian(color albedo);

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& atteuation, ray& scattered) const override;

public:
	//������
	color albedo;
};

//����
class metal : public material {
public:
	metal() : albedo(color(1.0, 1.0, 1.0)), roughness(0.0) {}
	metal(color albedo, double roughness);

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& atteuation, ray& scattered) const override;

public:
	//������
	color albedo;

	//�ֲڶ�
	double roughness;
};

//��������
class dielectric : public material {
public:
	dielectric() : ref_rdx(1.0) {}
	dielectric(double ri);

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& atteuation, ray& scattered) const override;
	double schlick(double cosine, double ri) const;

private:
	double ref_rdx;
};

#endif // !MATERIAL_H
