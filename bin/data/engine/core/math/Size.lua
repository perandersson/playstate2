local Size = {}

function Size.__call(width, height)
	return {width, height}
end

function Size.DivSingle(vec, value)
	return { vec[1] / value, vec[2] / value }
end

return Size