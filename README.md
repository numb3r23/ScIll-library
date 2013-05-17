ScIll-library
=============

3D-rendering library for _OpenGL 3.2 core_ that was created for rendering scientific illustrations but can be used for lots of things...

Certain aspects were inspired by Qt-OpenGL, which unfortunately doesn't do _OpenGL 3.2 core_ too well but offers nice abstractions. 

A key-aspect is the `FilterEngine` that allows for very easy/simple fragment-shader image filters or deferred-rendering setups. The following code illustrates rendering into a rendertarget:

> using namespace SciIllLib;
>  
> CFilter fltFXAA = new CFilter();  
> fltFXAA->LoadFragmentShader("res/glsl/filter/FXAA.frag");
> 
> SFilterEngine::ReGenerateTexture(&m_rtScene, GL_FLOAT);
> SFilterEngine::ReGenerateTexture(&m_rtFinal, GL_FLOAT);
>  
> SFilterEngine::PreRender(m_rtScene);  
> ... render as usual ...  
> SFilterEngine::PostRender();  
>  
> SFilterEngine::Apply(fltFXAA, m_rtScene, m_rtFinal);  
> SFilterEngine::ToScreen(fltDisplay, m_rtFinal);  


### soon ###
-  more pass-by-references parameters, currently rather old/lazy implementation
-  one or more running examples
-  more features (Postprocessors, Filters, )