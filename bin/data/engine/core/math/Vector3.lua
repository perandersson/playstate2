local Vector3 = {}
function Vector3.__call(x, y, z)
	return {x, y, z}
end

function Vector3.Add(lhs, rhs)
	return { lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + rhs[3] }
end

function Vector3.Sub(lhs, rhs)
	return { lhs[1] - rhs[1], lhs[2] - rhs[2], lhs[3] - rhs[3] }
end

function Vector3.MultSingle(lhs, rhs)
	return { lhs[1] * rhs, lhs[2] * rhs, lhs[3] * rhs }
end

function Vector3.Length(vec)
	return math.sqrt(vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3])
end

function Vector3.Normalize(vec)
	local lengthMul = 1.0 / Vector3.Length(vec)
	return { vec[1] * lengthMul, vec[2] * lengthMul, vec[3] * lengthMul }
end

function Vector3.DivSingle(vec, rhs)
	return { vec[1] / rhs, vec[2] / rhs, vec[3] / rhs }
end

function Vector3.SquaredLength(vec)
	return vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3]
end

return Vector3