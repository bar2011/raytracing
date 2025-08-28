#include <metal_stdlib>
using namespace metal;

struct Ray {
  float3 origin;
  float3 direction;
  
  half3 skyboxColor() const {
    const float scaledY = (direction.y + 1.0f) * 0.5f;
    constexpr half3 bottomSkybox = half3(1.0f, 1.0f, 1.0f);
    constexpr half3 topSkybox = half3(0.5f, 0.7f, 1.0f);
    return (1 - scaledY) * bottomSkybox + scaledY * topSkybox;
  }
  
  float3 at(float t) const {
    return origin + direction * t;
  }
};

struct Intersection {
  float time;
  float3 point;
  float3 normal;
  bool frontFace;
  
  // NOTE: the parameter `outward_normal` is assumed to be normalized.
  void setFaceNormal(const thread Ray &ray, float3 outwardNormal) {
    frontFace = dot(ray.direction, outwardNormal) < 0.0;
    normal = frontFace ? outwardNormal : -outwardNormal;
  }
};

struct Sphere {
  float3 center;
  float radius;
  half3 color;
  
  // ray - the ray that intersection is checked upon
  // intersection is set if one is found, with the details of the intersection
  // returns whether the ray intersected the sphere or not
  // NOTE: the field ray.direction is assumed to be normalized.
  bool intersect(const thread Ray &ray, thread Intersection &intersection) const {
    const float3 originToCenter = center - ray.origin;
    const float timeIntersectCenter = dot(originToCenter, ray.direction);
    if (timeIntersectCenter < 0) return false;
    
    const float centerToRayLengthSquared = dot(originToCenter, originToCenter) - timeIntersectCenter * timeIntersectCenter;
    const float radiusSquared = radius * radius;
    
    const float discriminant = radiusSquared - centerToRayLengthSquared;
    
    if (discriminant < 0) return false;
    if (discriminant == 0) {
      intersection = Intersection{
        .time=timeIntersectCenter,
        .point=ray.at(timeIntersectCenter)
      };
      
      // P is on the sphere, so |P-C|=R, so dividing by R makes it normalized
      float3 normal = (intersection.point - center) / radius;
      
      intersection.setFaceNormal(ray, normal);
      
      return true;
    }

    const float intersectTimeVariation = discriminant * rsqrt(discriminant);
    
    float t1 = timeIntersectCenter - intersectTimeVariation;
    t1 = t1 < 0 ? INFINITY : t1;
    float t2 = timeIntersectCenter + intersectTimeVariation;
    t2 = t2 < 0 ? INFINITY : t2;

    const float time = min(t1, t2); // time is the smallest of the two times

    intersection = Intersection{
      .time=time,
      .point=ray.at(time)
    };
    
    // P is on the sphere, so |P-C|=R, so dividing by R makes it normalized
    float3 normal = (intersection.point - center) / radius;
    
    intersection.setFaceNormal(ray, normal);
    
    return true;
  }
};

struct Object {
  enum class Type {
    Sphere,
  };
  
  Type type;
  union {
    Sphere sphere;
  };
  
  // ray - the ray that intersection is checked upon
  // intersection is set if one is found, with the details of the intersection
  // returns whether the ray intersected the sphere or not
  // NOTE: the field ray.direction is assumed to be normalized.
  bool intersect(const thread Ray &ray, thread Intersection &intersection) const {
    switch (type) {
      case Type::Sphere: return sphere.intersect(ray, intersection);
    }
  }
};

struct Camera {
  float3 right;
  float3 up;
  float3 forward;
  float3 pos;
};

kernel void tracer(
  texture2d<half, access::write> outTexture [[texture(0)]],
  constant unsigned long* shaderIter [[buffer(0)]],
  constant Camera* camera [[buffer(1)]],
  uint2 gid [[thread_position_in_grid]]) {

  const float width = float(outTexture.get_width());
  const float height = float(outTexture.get_height());

  if (gid.x >= width || gid.y >= height) return;
  
  const float aspect = width / height;
  const float focalLength = 1.5f;

  const float2 ndc = float2((0.5f + float(gid.x)) / width, (0.5f + float(gid.y)) / height);
  float2 screen = 2.0 * ndc - 1.0;
  screen.x *= aspect;
  
  const float3 rayDir = normalize(screen.x * camera->right + screen.y * camera->up + camera->forward * focalLength);
  Ray ray = Ray{camera->pos, rayDir};
  
  constexpr Object objects[] = {
      Object{
          .type = Object::Type::Sphere,
          .sphere = Sphere{
              .center = float3(0.0, 0.0, -5.0),
              .radius = 1.0,
              .color = half3(1.0, 0.0, 0.0) // red
          }
      },
      Object{
          .type = Object::Type::Sphere,
          .sphere = Sphere{
              .center = float3(2.0, 0.0, -6.0),
              .radius = 0.5,
              .color = half3(0.0, 1.0, 0.0) // green
          }
      },
      Object{
          .type = Object::Type::Sphere,
          .sphere = Sphere{
              .center = float3(-1.5, 1.0, -4.0),
              .radius = 0.75,
              .color = half3(0.0, 0.0, 1.0) // blue
          }
      },
      Object{
          .type = Object::Type::Sphere,
          .sphere = Sphere{
              .center = float3(3.0, 1.0, 4.0),
              .radius = 2.0,
              .color = half3(1.0, 1.0, 0.0) // yellow
      }
    },
  };

  Intersection closestIntersection = { .time = INFINITY };
  for (size_t i = 0; i < 3; ++i) {
    Intersection intersection;
    if (objects[i].intersect(ray, intersection) && intersection.time < closestIntersection.time)
      closestIntersection = intersection;
  }
  if (closestIntersection.time != INFINITY)
    outTexture.write(half4(half3((closestIntersection.normal + 1.0) / 2.0), 1.0), gid);
  else
    outTexture.write(half4(ray.skyboxColor(), 1.0), gid);

  // outTexture.write(half4(half3(rayDir), 1.0), gid);
}
