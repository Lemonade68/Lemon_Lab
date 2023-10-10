### Lemon_Lab_Experiment_Version

构建整体框架，暂时采用ppm图像输出（进度条学习moer-lite）、暂时不支持json导入

参考：moer-lite; ray-tracing series; moer; mitsuba; pbrt

- [x] main
- [x] ray
- [x] camera
  - [x] pinhole
  - [ ] thin lens

- [ ] light
  - [ ] AreaLight
  - [ ] EnvironmentLight
  - [ ] SpotLight

- [ ] math(vec/matrix)     考虑其他库？
  - [x] glm库 (**注意矩阵是列主序**)
  - [ ] fastmath？

- [x] transform
  - [ ] TODO：包围盒部分
  - [ ] 

- [ ] shape
  - [ ] 材质部分初始化
  - [x] Intersection
  - [x] sphere
  - [x] parallelogram（可用作地面和光源）
    - [ ] 光源设置未完成
  - [ ] triangle
  - [ ] cube
  
- [ ] bounding box 
  - [ ] AABB

- [ ] acceleration
  - [ ] BVH
  - [ ] EmbreeBVH
  
- [ ] integrator
  - [x] normal (for testing)
  - [ ] directIntegrator
  - [ ] whittedIntegrator
  - [ ] pathIntegrator

- [x] sampler
  - [x] random
  - [x] IndependentSampler

- [ ] material
  - [ ] Matte
  - [ ] Mirror
  - [ ] Blinn_Phong
  - [ ] conductor
  - [ ] dielectric
  - [ ] BXDF

- [ ] texture
  - [ ] constantTexture
  - [ ] imageTexture
  - [ ] normalTexture

- [ ] scene(写好加速结构后写)
- [ ] filter
- [ ] medium

待做：思考为什么原来的sphere那里有问题

