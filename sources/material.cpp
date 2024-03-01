#include "material.h"

lambertian::lambertian(color albedo) : albedo(albedo) {}
bool lambertian::scatter(const ray& r_in, const hit_record &rec, color& atteuation, ray& scattered) const {
	auto scatter_direction = rec.normal + random_unit_vector();

	if (scatter_direction.near_zero()) scatter_direction = rec.normal;

	scattered = ray(rec.p3, scatter_direction);
	atteuation = albedo;
	return true;
}

metal::metal(color albedo, double r) : albedo(albedo), roughness(r > 1 ? 1 : r) {}
bool metal::scatter(const ray& r_in, const hit_record& rec, color& atteuation, ray& scattered) const {
	vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal + roughness * random_in_unit_sphere());
	scattered = ray(rec.p3, reflected);
	atteuation = albedo;
	return (dot(scattered.direction(), rec.normal) > 0);
}

dielectric::dielectric(double ri) : ref_rdx(ri) {}
bool dielectric::scatter(const ray& r_in, const hit_record& rec, color& atteuation, ray& scattered) const {
	atteuation = color(1.0, 1.0, 1.0);
	double etai_over_etat = (rec.front_face) ? (1.0 / ref_rdx) : ref_rdx;

	vec3 unit_direction = unit_vector(r_in.direction());
	double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
	double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);
	if (etai_over_etat * sin_theta > 1.0) {
		vec3 reflected = reflect(unit_direction, rec.normal);
		scattered = ray(rec.p3, reflected);
		return true;
	}

	double reflect_prob = schlick(cos_theta, etai_over_etat);
	if (random_double() < reflect_prob)
	{
		vec3 reflected = reflect(unit_direction, rec.normal);
		scattered = ray(rec.p3, reflected);
		return true;
	}

	vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
	scattered = ray(rec.p3, refracted);
	return true;
}

double dielectric::schlick(double cosine, double ri) const {
	auto r0 = (1 - ri) / (1 + ri);
	r0 = r0*r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}