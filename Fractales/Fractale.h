#pragma once

#include <iostream>
#include <string>
#include <imgui.h>

#include "shader.h"

extern int selectedFractal;

class Fractale {
protected:
	std::string pathVertexShader = "shaders/vertex.vs";
	bool m_smooth = true;
	int m_iterations = 40;
	float m_colorRange = 0.017f;
	int m_id = 0;

	std::vector<Shader*> shaders;
	int activeShader = 0;
public:
	void setActiveShader(int id) {
		activeShader = id;
	}
	virtual void Render() = 0;
	virtual void Menu();
	virtual Shader* getShader() = 0;
	virtual std::pair<float, float> getCoordsForZoom() = 0;
};

class Mandelbrot : public Fractale {
	std::vector<Shader*> shaders = {
		new Shader(pathVertexShader, "shaders/mandelbrots/mandelbrot_Original.fs"),
		new Shader(pathVertexShader, "shaders/mandelbrots/mandelbrot_Fire.fs"),
		new Shader(pathVertexShader, "shaders/mandelbrots/mandelbrot_Electric.fs"),
		new Shader(pathVertexShader, "shaders/mandelbrots/mandelbrot_Gold.fs"),
		new Shader(pathVertexShader, "shaders/mandelbrots/mandelbrot_Verdoyante.fs"),
		new Shader(pathVertexShader, "shaders/mandelbrots/mandelbrot_Perle.fs"),
	};
public:
	Mandelbrot();
	void Render();
	void Menu() override;
	Shader* getShader() override;
	std::pair<float, float> getCoordsForZoom();
};

class Julia : public Fractale {
	std::vector<Shader*> shaders = {
		new Shader(pathVertexShader, "shaders/julias/julia_Original.fs"),
		new Shader(pathVertexShader, "shaders/julias/julia_Fire.fs"),
		new Shader(pathVertexShader, "shaders/julias/julia_Electric.fs"),
		new Shader(pathVertexShader, "shaders/julias/julia_Gold.fs"),
		new Shader(pathVertexShader, "shaders/julias/julia_Verdoyante.fs"),
		new Shader(pathVertexShader, "shaders/julias/julia_Perle.fs")
	};
	float m_v1 = 0.311f;
	float m_v2 = -0.026f;
public:
	Julia();
	void Render();
	void Menu() override;
	Shader* getShader() override;
	std::pair<float, float> getCoordsForZoom();
};

class BurningShip : public Fractale {
	std::vector<Shader*> shaders = {
		new Shader(pathVertexShader, "shaders/burningships/burningship_Original.fs"),
		new Shader(pathVertexShader, "shaders/burningships/burningship_Fire.fs"),
		new Shader(pathVertexShader, "shaders/burningships/burningship_Electric.fs"),
		new Shader(pathVertexShader, "shaders/burningships/burningship_Gold.fs"),
		new Shader(pathVertexShader, "shaders/burningships/burningship_Verdoyante.fs"),
		new Shader(pathVertexShader, "shaders/burningships/burningship_Perle.fs")
	};
public:
	BurningShip();
	void Render();
	void Menu() override;
	Shader* getShader() override;
	std::pair<float, float> getCoordsForZoom();
};

class Tricorn : public Fractale {
	std::vector<Shader*> shaders = {
		new Shader(pathVertexShader, "shaders/tricorns/tricorn_Original.fs"),
		new Shader(pathVertexShader, "shaders/tricorns/tricorn_Fire.fs"),
		new Shader(pathVertexShader, "shaders/tricorns/tricorn_Electric.fs"),
		new Shader(pathVertexShader, "shaders/tricorns/tricorn_Gold.fs"),
		new Shader(pathVertexShader, "shaders/tricorns/tricorn_Verdoyante.fs"),
		new Shader(pathVertexShader, "shaders/tricorns/tricorn_Perle.fs")
	};
public:
	Tricorn();
	void Render();
	void Menu() override;
	Shader* getShader() override;
	std::pair<float, float> getCoordsForZoom();
};

class Multibrot : public Fractale {
	int m_numberOfBrot = 4;
	std::vector<Shader*> shaders = {
		new Shader(pathVertexShader, "shaders/multibrots/multibrot_Original.fs"),
		new Shader(pathVertexShader, "shaders/multibrots/multibrot_Fire.fs"),
		new Shader(pathVertexShader, "shaders/multibrots/multibrot_Electric.fs"),
		new Shader(pathVertexShader, "shaders/multibrots/multibrot_Gold.fs"),
		new Shader(pathVertexShader, "shaders/multibrots/multibrot_Verdoyante.fs"),
		new Shader(pathVertexShader, "shaders/multibrots/multibrot_Perle.fs")
	};
public:
	Multibrot();
	void Render();
	void Menu() override;
	Shader* getShader() override;
	std::pair<float, float> getCoordsForZoom();
};