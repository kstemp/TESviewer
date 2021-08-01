#pragma once

#include "..\Record.h"

namespace ESM {
	namespace RecordType {
		constexpr RecordTypeVal LIGH = 66;
	}

	struct LightData {
		int32_t time;
		uint32_t radius;
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
		uint32_t flags;
		float falloff;
		float FOV;
		float nearClip;
		float invPeriod;
		float intensityAmplitude;
		float movementAmplitude;
		uint32_t value;
		float weight;
	};

	struct LIGH : Record {
		std::string MODL;

		LIGH() : Record("LIGH", RecordType::LIGH) {}

		LightData data;

		virtual void parseField(BinaryStreamReader& bsr, const std::string& fieldName, const uint16_t fieldSize) override {
			if (fieldName == "MODL")
				MODL = bsr.readString(fieldSize);
			else if (fieldName == "EDID")
				EDID = bsr.readString(fieldSize);
			else if (fieldName == "DATA") {
				bsr >> data.time >> data.radius;
				bsr >> data.r >> data.g >> data.b;
				bsr >> data.a;
				bsr >> flags;
				bsr >> data.falloff >> data.FOV >> data.nearClip;
				bsr >> data.invPeriod >> data.intensityAmplitude >> data.movementAmplitude;
				bsr >> data.value >> data.weight;
			}
			else
				bsr.skip(fieldSize);
		}

		std::optional<std::string> model() const override {
			return MODL;
		}
	};
}