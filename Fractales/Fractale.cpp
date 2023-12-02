#include "Fractale.h"
#include <GLFW/glfw3.h>

void Fractale::Menu()
{
	ImGui::Columns(3, "Fractals");
	ImGui::RadioButton("Mandelbrot", &selectedFractal, 0);
	ImGui::NextColumn();
	ImGui::RadioButton("Julia", &selectedFractal, 1);
	ImGui::NextColumn();
	ImGui::RadioButton("Burning Ship", &selectedFractal, 2);
	ImGui::NextColumn();
	ImGui::Columns(1);

	ImGui::NewLine();
	ImGui::SliderInt("Iterations", &m_iterations, 20, 5000);

	ImGui::NewLine();
	ImGui::SliderFloat("Color Range", &m_colorRange, 0.01f, 10.f);

	ImGui::Separator();
	ImGui::Checkbox("Smooth", &m_smooth);
}

Mandelbrot::Mandelbrot()
{
	m_id = 0;
}

void Mandelbrot::Render()
{
	shaders[activeShader]->setFloat("maxIter", m_iterations);
	shaders[activeShader]->setBool("smooth_color", m_smooth);
	shaders[activeShader]->setFloat("colorRange", m_colorRange);
	shaders[activeShader]->use();
}

void Mandelbrot::Menu()
{
	Fractale::Menu();
}

Shader* Mandelbrot::getShader()
{
	return shaders[activeShader];
}

std::pair<float, float> Mandelbrot::getCoordsForZoom()
{
	return std::make_pair(0.4, 0.35);
}

Julia::Julia()
{
	m_id = 1;
	m_iterations = 150;
}

void Julia::Render()
{
	shaders[activeShader]->setFloat("maxIter", m_iterations);
	shaders[activeShader]->setBool("smooth_color", m_smooth);
	shaders[activeShader]->setFloat("colorRange", m_colorRange);
	shaders[activeShader]->setFloat("v1", m_v1);
	shaders[activeShader]->setFloat("v2", m_v2);
	shaders[activeShader]->use();
}

void Julia::Menu()
{
	Fractale::Menu();
	ImGui::NewLine();
	ImGui::Text("Julia");
	ImGui::SliderFloat("v1", &m_v1, -2.f, 2.f);
	ImGui::SliderFloat("v2", &m_v2, -2.f, 2.f);
}

Shader* Julia::getShader()
{
	return shaders[activeShader];
}

std::pair<float, float> Julia::getCoordsForZoom()
{
	return std::make_pair(-0.0175, -0.453333);
}

BurningShip::BurningShip()
{
	m_id = 2;
	m_iterations = 300;
}

void BurningShip::Render()
{
	shaders[activeShader]->setBool("smooth_color", m_smooth);
	shaders[activeShader]->setFloat("colorRange", m_colorRange);
	shaders[activeShader]->setFloat("maxIter", m_iterations);
	shaders[activeShader]->use();
}

void BurningShip::Menu()
{
	Fractale::Menu();
}

Shader* BurningShip::getShader()
{
	return shaders[activeShader];
}

std::pair<float, float> BurningShip::getCoordsForZoom()
{
	return std::make_pair(-1.762, -0.02);
}