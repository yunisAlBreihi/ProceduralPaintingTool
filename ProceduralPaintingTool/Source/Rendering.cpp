#include "Rendering.h"
#include "globals.h"
#include "Mesh.h"

void mesh_draw(const Mesh& mesh, const RenderData& data) {
	glBindVertexArray(mesh.m_vao);

	//Set material
	data.m_shader->use();

	//Set matrices
	data.m_shader->set_attribute("u_Model", data.m_model);
	data.m_shader->set_attribute("u_ViewProjection", data.m_view_projection);

	//Set lights
	data.m_shader->set_attribute("u_DirectionalLight", data.m_directional_light);
	data.m_shader->set_attribute("u_EyePosition", data.m_eye_position);

	glDrawArrays(GL_TRIANGLES, 0, mesh.m_draw_count);
}

void debugLine_draw(const DebugLine& debugLine, const RenderData& data) {
	glBindVertexArray(debugLine.m_vao);

	//Set material
	data.m_shader->use();

	glDrawArrays(GL_LINES, 0, debugLine.m_draw_count);
}