/*
 * opencog/atoms/core/NumberNode.cc
 *
 * Copyright (C) 2015 Linas Vepstas
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include <sstream>

#include "NumberNode.h"

using namespace opencog;

/// Convert vector to json-format string
/// Why json? I dunno.
std::string NumberNode::vector_to_json(const std::vector<double>& vec)
{
	std::stringstream ss;
	ss << "[";
	for (double v: vec) ss << double_to_string(v) << ", ";
	ss << "]";
	return ss.str();
}

std::string NumberNode::vector_to_plain(const std::vector<double>& vec)
{
	std::stringstream ss;
	for (double v: vec) ss << double_to_string(v) << " ";
	return ss.str();
}

/// Support multiple formats:
///   plain)   "0.1 0.2 0.3"
///   csv)     "0.1, 0.2, 0.3"
///   scheme)  "#(0.1 0.2 0.3)"
///   json)    "[0.1, 0.2, 0.3]"
/// Why multiple formats? I dunno.
/// Currently, only "plain" is supported.
std::vector<double> NumberNode::to_vector(const std::string& str)
{
	std::vector<double> vec;

	size_t pos = 0;
	size_t len = str.size();
	while (true)
	{
		pos = str.find_first_of("0123456789.", pos);
		if (pos == std::string::npos) return vec;
		size_t last;
		vec.emplace_back(std::stod(str.substr(pos), &last));
		if (pos == std::string::npos) return vec;
		pos += last;
		if (len <= pos) return vec;
	}
	return vec;
}

// ============================================================
// Constructors

NumberNode::NumberNode(Type t, const std::string& s)
	: Node(t, s)
{
	// Convert to number and back to string to avoid miscompares.
	_value = to_vector(s);
	_name = vector_to_plain(_value);

	OC_ASSERT(nameserver().isA(_type, NUMBER_NODE),
		"Bad NumberNode constructor!");
}

NumberNode::NumberNode(const std::string& s)
	: Node(NUMBER_NODE, s)
{
	_value = to_vector(s);
	_name = vector_to_plain(_value);
}

NumberNode::NumberNode(Node &n)
	: Node(n)
{
	OC_ASSERT(nameserver().isA(_type, NUMBER_NODE),
		"Bad NumberNode constructor!");

	_value = to_vector(n.get_name());
	_name = vector_to_plain(_value);
}

NumberNode::NumberNode(const std::vector<double>& vec)
	: Node(NUMBER_NODE, "")
{
	_value = vec;
	_name = vector_to_plain(_value);
}

NumberNode::NumberNode(const FloatValuePtr& fv)
	: Node(NUMBER_NODE, "")
{
	_value = fv->value();
	_name = vector_to_plain(_value);
}

// ============================================================
// Vector ops
// There's a lot of cut-n-paste here, maybe it can be reduced.

/// Scalar multiplication
ValuePtr opencog::times(double scalar, const NumberNodePtr& fvp)
{
	const std::vector<double>& fv = fvp->value();
	size_t len = fv.size();
	std::vector<double> prod(len);
	for (size_t i=0; i<len; i++)
		prod[i] = scalar * fv[i];

	return createFloatValue(prod);
}

/// Scalar addition
ValuePtr opencog::plus(double scalar, const NumberNodePtr& fvp)
{
	const std::vector<double>& fv = fvp->value();
	size_t len = fv.size();
	std::vector<double> sum(len);
	for (size_t i=0; i<len; i++)
		sum[i] = scalar + fv[i];

	return createFloatValue(sum);
}

/// Scalar division
ValuePtr opencog::divide(double scalar, const NumberNodePtr& fvp)
{
	const std::vector<double>& fv = fvp->value();
	size_t len = fv.size();
	std::vector<double> ratio(len);
	for (size_t i=0; i<len; i++)
		ratio[i] = scalar / fv[i];

	return createFloatValue(ratio);
}

/// Vector (point-wise) multiplication
ValuePtr opencog::times(const NumberNodePtr& fvpa, const NumberNodePtr& fvpb)
{
	const std::vector<double>& fva = fvpa->value();
	const std::vector<double>& fvb = fvpb->value();
	size_t len = fva.size();
	size_t lenb = fvb.size();
	if (lenb < len) len = lenb;

	std::vector<double> prod(len);
	for (size_t i=0; i<len; i++)
		prod[i] = fva[i] * fvb[i];

	return createFloatValue(prod);
}

/// Vector (point-wise) addition
ValuePtr opencog::plus(const NumberNodePtr& fvpa, const NumberNodePtr& fvpb)
{
	const std::vector<double>& fva = fvpa->value();
	const std::vector<double>& fvb = fvpb->value();
	size_t len = fva.size();
	size_t lenb = fvb.size();
	if (lenb < len) len = lenb;

	std::vector<double> sum(len);
	for (size_t i=0; i<len; i++)
		sum[i] = fva[i] + fvb[i];

	return createFloatValue(sum);
}

/// Vector (point-wise) division
ValuePtr opencog::divide(const NumberNodePtr& fvpa, const NumberNodePtr& fvpb)
{
	const std::vector<double>& fva = fvpa->value();
	const std::vector<double>& fvb = fvpb->value();
	size_t len = fva.size();
	size_t lenb = fvb.size();
	if (lenb < len) len = lenb;

	std::vector<double> ratio(len);
	for (size_t i=0; i<len; i++)
		ratio[i] = fva[i] / fvb[i];

	return createFloatValue(ratio);
}

/// Vector (point-wise) multiplication
ValuePtr opencog::times(const NumberNodePtr& fvpa, const FloatValuePtr& fvpb)
{
	const std::vector<double>& fva = fvpa->value();
	const std::vector<double>& fvb = fvpb->value();
	size_t len = fva.size();
	size_t lenb = fvb.size();
	if (lenb < len) len = lenb;

	std::vector<double> prod(len);
	for (size_t i=0; i<len; i++)
		prod[i] = fva[i] * fvb[i];

	return createFloatValue(prod);
}

/// Vector (point-wise) addition
ValuePtr opencog::plus(const NumberNodePtr& fvpa, const FloatValuePtr& fvpb)
{
	const std::vector<double>& fva = fvpa->value();
	const std::vector<double>& fvb = fvpb->value();
	size_t len = fva.size();
	size_t lenb = fvb.size();
	if (lenb < len) len = lenb;

	std::vector<double> sum(len);
	for (size_t i=0; i<len; i++)
		sum[i] = fva[i] + fvb[i];

	return createFloatValue(sum);
}

/// Vector (point-wise) division
ValuePtr opencog::divide(const NumberNodePtr& fvpa, const FloatValuePtr& fvpb)
{
	const std::vector<double>& fva = fvpa->value();
	const std::vector<double>& fvb = fvpb->value();
	size_t len = fva.size();
	size_t lenb = fvb.size();
	if (lenb < len) len = lenb;

	std::vector<double> ratio(len);
	for (size_t i=0; i<len; i++)
		ratio[i] = fva[i] / fvb[i];

	return createFloatValue(ratio);
}

/// Vector (point-wise) multiplication
ValuePtr opencog::times(const FloatValuePtr& fvpa, const NumberNodePtr& fvpb)
{
	const std::vector<double>& fva = fvpa->value();
	const std::vector<double>& fvb = fvpb->value();
	size_t len = fva.size();
	size_t lenb = fvb.size();
	if (lenb < len) len = lenb;

	std::vector<double> prod(len);
	for (size_t i=0; i<len; i++)
		prod[i] = fva[i] * fvb[i];

	return createFloatValue(prod);
}

/// Vector (point-wise) addition
ValuePtr opencog::plus(const FloatValuePtr& fvpa, const NumberNodePtr& fvpb)
{
	const std::vector<double>& fva = fvpa->value();
	const std::vector<double>& fvb = fvpb->value();
	size_t len = fva.size();
	size_t lenb = fvb.size();
	if (lenb < len) len = lenb;

	std::vector<double> sum(len);
	for (size_t i=0; i<len; i++)
		sum[i] = fva[i] + fvb[i];

	return createFloatValue(sum);
}

/// Vector (point-wise) division
ValuePtr opencog::divide(const FloatValuePtr& fvpa, const NumberNodePtr& fvpb)
{
	const std::vector<double>& fva = fvpa->value();
	const std::vector<double>& fvb = fvpb->value();
	size_t len = fva.size();
	size_t lenb = fvb.size();
	if (lenb < len) len = lenb;

	std::vector<double> ratio(len);
	for (size_t i=0; i<len; i++)
		ratio[i] = fva[i] / fvb[i];

	return createFloatValue(ratio);
}

// ============================================================

DEFINE_NODE_FACTORY(NumberNode, NUMBER_NODE)
