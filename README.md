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
  - [x] TODO：包围盒部分
  - [ ] 
- [ ] shape
  - [x] 材质部分初始化
  - [x] Intersection
  - [x] sphere
  - [x] parallelogram（可用作地面和光源）
    - [x] 光源设置未完成
    - [x] 存在沿着平行四边形两边无限延长的问题，添加解决方法后边的长度不影响大小，只有边的方向起作用（**改用自己的方法**）
  - [x] triangle
    - [x] triangle mesh
  - [x] cube
- [x] bounding box 
  - [x] AABB
- [x] acceleration
  - [x] BVH
  - [ ] EmbreeBVH
- [ ] integrator
  - [x] 光线距离平方衰减
  - [x] normal (for testing)
  - [x] directIntegrator（环境光部分等待完善 —— environment map等）
    - [x] sampleBSDF(球那里有点实现上的问题 —— 已解决)
    - [x] sampleLight
  - [x] whittedIntegrator（不限制反射上限的话可能会进入死循环，看日志）
  - [x] pathIntegrator（蒙特卡洛积分器）
    - [x] 光强变大，需要HDR图片输出支持
    - [ ] 目前仍有缺陷（有近似），带后续改进，详情见pathIntegrator.cpp
  - [ ] BDPT
- [x] sampler
  - [x] random
  - [x] IndependentSampler
- [ ] material
  - [x] Matte(对应Lambert)
  - [x] Mirror(direct integrator sample light那里的f直接返回0，看后续是否需要改进)
  - [ ] Blinn_Phong
  - [ ] conductor
    - [x] 目前roughness只使用了单一的float表示，后续改成vector2f，用于各向异性材料使用
  - [ ] dielectric
    - [x] roughDielectric，当alpha较大时噪点较多，再考虑为什么
      - [ ] BXDF
        - [ ] sampling方法（返回wi和pdf）
      - [x] 均匀采样
      - [ ] 重要性采样
    - [x] cosine权重采样（Lambert）
    - [x] brdf采样
    - [x] Specular
    - [ ] Phong
    - [x] Conductor
    - [x] Dielectric
- [ ] texture
  - [x] constantTexture
  - [x] imageTexture
  - [x] CheckerTexture
  - [ ] normalTexture
- [ ] scene(目前使用的是std::vector\<std::shared_ptr\<Shape>> Shape_list 的Scene类)
  - [ ] 加速结构（后续改进为该结构的Scene）
    - [x] BVH
    - [ ] EmbreeBVH
    - [ ] Octree
- [ ] filter
- [ ] medium
- [ ] Json文件录入支持
- [ ] 导入模型
  - [x] 单个mesh模型导入+纹理
  - [x] 多个mesh的obj模型导入
  - [ ] gltf格式场景导入
  - [ ] 模型的mtl文件导入以及材质的设置



Mipmap相关事宜

TODO：Sphere的旋转（应该是在构造函数添加即可，不对，需要添加球的默认方向，Film，Blinn_Phong材质，Conductor/Dielectric(PBR)，**JPG/HDR图片写入（stb_image_write.h）**，动态模糊（在写好AABB+BVH后写），尝试导入gltf、fbx这种大场景模型，环境光（EnvironmentLight、InfiniteLight等），球体的scale来实现缩放，**球的采样（rt series第三本中有）**

图片写入：Image类，写入



Embree实现：intersection中直接添加primID，看是否可行；rayIntersectShape参数更改，uv的记录；triangle mesh的内部构造那边shapelist有问题
