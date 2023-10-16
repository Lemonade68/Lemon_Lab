### Lemon_Lab_Experiment_Version

构建整体框架，暂时采用ppm图像输出（进度条学习moer-lite）、暂时不支持json导入

参考：moer-lite; ray-tracing series; moer; mitsuba; pbrt

- [x] main
- [x] ray
- [x] camera
  - [x] pinhole
  - [ ] thin lens

- [ ] light
  - [x] AreaLight
  - [ ] EnvironmentLight
  - [ ] SpotLight

- [ ] math(vec/matrix)     考虑其他库？
  - [x] glm库 (**注意矩阵是列主序，length()函数返回的是向量的元素个数，不是长度**)
  - [ ] fastmath？

- [x] transform
  - [ ] TODO：包围盒部分
  - [ ] 

- [ ] shape
  - [ ] 材质部分初始化
  - [x] Intersection
  - [x] sphere
  - [x] parallelogram（可用作地面和光源）
    - [x] 光源设置未完成
    - [x] 存在沿着平行四边形两边无限延长的问题，添加解决方法后边的长度不影响大小，只有边的方向起作用（**改用自己的方法**）
  - [ ] triangle
  - [ ] cube
  
- [ ] bounding box 
  - [ ] AABB

- [ ] acceleration
  - [ ] BVH
  - [ ] EmbreeBVH
  
- [ ] integrator
  - [x] 光线距离平方衰减
  - [x] normal (for testing)
  - [x] directIntegrator（环境光部分等待完善 —— environment map等）
    - [x] sampleBSDF(球那里有点实现上的问题 —— 已解决)
    - [x] sampleLight
  - [ ] whittedIntegrator
  - [ ] pathIntegrator
  
- [x] sampler
  - [x] random
  - [x] IndependentSampler

- [ ] material
  - [x] Matte(对应Lambert)
  - [x] Mirror(direct integrator sample light那里的f直接返回0，看后续是否需要改进)
  - [ ] Blinn_Phong
  - [ ] conductor
  - [ ] dielectric
  - [ ] BXDF
    - [x] Lambert
    - [x] Specular
    - [ ] Phong
    - [ ] Conductor
    - [ ] Dielectric
  
- [ ] texture
  - [ ] constantTexture
  - [ ] imageTexture
  - [ ] normalTexture

- [ ] scene(目前使用的是std::vector\<std::shared_ptr\<Shape>> Shape_list 的Scene类)
  - [ ] 加速结构（后续改进为该结构的Scene）

- [ ] filter
- [ ] medium
- [ ] Json文件录入支持
- [ ] 

看材质返回的BSDF是不是已经计算过cos项

Mipmap相关事宜

Matte材质中为什么需要重新算出新的albedo？

TODO：Texture(物体颜色)，Film，WhittedIntegrator，BVH，

先写texture（常量颜色），再写whittedIntegrator
