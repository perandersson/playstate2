#pragma once
#include "../typedefs.h"
#include "../math/Point.h"
#include "../math/Color.h"
#include "../math/Vector2.h"

namespace core
{
	/*!
		\brief The IConfiguration is used to access the game's configuration file. 

		IConfiguration is a service interface, which means to you can access it through the Configuration class or by using:
		{@code 
			IConfiguration* configuration = Kernel::GetConfiguration();
		}

		Example:
		{@code
			// Using the service interface directly
			IConfiguration* configuration = Kernel::GetConfiguration();
			Size windowSize = configuration->ToSize("window.size", Size(1024, 768));

			// Using the helper class
			Size windowSize = Configuration::ToSize("window.size", Size(1024, 768));
		}
	*/
	class IConfiguration
	{
	public:
		virtual ~IConfiguration() {}

		/*!
			\brief Retrieves an 32-bit integer value from the configuration

			\param key The configuration value key
			\param defaultValue The returned value if the supplied key doesn't exist.
			\return The configuration value
		*/
		virtual int32 ToInt32(const char* key, int32 defaultVal = 0) const = 0;

		/*!
			\brief Retrieves an 32-bit integer value from the configuration

			\param key The configuration value key
			\param defaultValue The returned value if the supplied key doesn't exist.
			\return The configuration value
		*/
		virtual int32 ToInt32(const std::string& key, int32 defaultVal = 0) const = 0;

		/*!
			\brief Retrieves an 32-bit single decimal value from the configuration

			\param key The configuration value key
			\param defaultValue The returned value if the supplied key doesn't exist.
			\return The configuration value
		*/
		virtual float32 ToFloat32(const char* key, float32 defaultVal = 0.0f) const = 0;

		/*!
			\brief Retrieves an 32-bit single decimal value from the configuration

			\param key The configuration value key
			\param defaultValue The returned value if the supplied key doesn't exist.
			\return The configuration value
		*/
		virtual float32 ToFloat32(const std::string& key, float32 defaultVal = 0.0f) const = 0;

		/*!
			\brief Retrieves a boolean value from the configuration

			\param key The configuration value key
			\param defaultValue The returned value if the supplied key doesn't exist.
			\return The configuration value
		*/
		virtual bool ToBool(const char* key, bool defaultVal = false) const = 0;

		/*!
			\brief Retrieves a boolean value from the configuration

			\param key The configuration value key
			\param defaultValue The returned value if the supplied key doesn't exist.
			\return The configuration value
		*/
		virtual bool ToBool(const std::string& key, bool defaultVal = false) const = 0;

		/*!
			\brief Retrieves a string value from the configuration

			\param key The configuration value key
			\param defaultValue The returned value if the supplied key doesn't exist.
			\return The configuration value
		*/
		virtual std::string ToString(const char* key, const char* defaultVal = "") const = 0;

		/*!
			\brief Retrieves a string value from the configuration

			\param key The configuration value key
			\param defaultValue The returned value if the supplied key doesn't exist.
			\return The configuration value
		*/
		virtual std::string ToString(const std::string& key, const char* defaultVal = "") const = 0;
		
		/*!
			\brief Retrieves a color from the configuration

			\param key The configuration value key
			\param defaultValue The returned value if the supplied key doesn't exist.
			\return The configuration value
		*/
		virtual Color ToColor(const char* key, const Color& defaultColor) const = 0;
		
		/*!
			\brief Retrieves a color from the configuration

			\param key The configuration value key
			\param defaultValue The returned value if the supplied key doesn't exist.
			\return The configuration value
		*/
		virtual Color ToColor(const std::string& key, const Color& defaultColor) const = 0;
		
		/*!
			\brief Retrieves a 2d vector from the configuration

			\param key The configuration value key
			\param defaultValue The returned value if the supplied key doesn't exist.
			\return The configuration value
		*/
		virtual Vector2 ToVector2(const char* key, const Vector2& defaultVector) const = 0;
		
		/*!
			\brief Retrieves a 2d vector from the configuration

			\param key The configuration value key
			\param defaultValue The returned value if the supplied key doesn't exist.
			\return The configuration value
		*/
		virtual Vector2 ToVector2(const std::string& key, const Vector2& defaultVector) const = 0;
		
		/*!
			\brief Retrieves a point from the configuration

			\param key The configuration value key
			\param defaultValue The returned value if the supplied key doesn't exist.
			\return The configuration value
		*/
		virtual Point ToPoint(const char* key, const Point& defaultVal) const = 0;
		
		/*!
			\brief Retrieves a point from the configuration

			\param key The configuration value key
			\param defaultValue The returned value if the supplied key doesn't exist.
			\return The configuration value
		*/
		virtual Point ToPoint(const std::string& key, const Point& defaultVal) const = 0;
		
		/*!
			\brief Retrieves a size from the configuration

			\param key The configuration value key
			\param defaultValue The returned value if the supplied key doesn't exist.
			\return The configuration value
		*/
		virtual Size ToSize(const char* key, const Size& defaultVal) const = 0;
		
		/*!
			\brief Retrieves a size from the configuration

			\param key The configuration value key
			\param defaultValue The returned value if the supplied key doesn't exist.
			\return The configuration value
		*/
		virtual Size ToSize(const std::string& key, const Size& defaultVal) const = 0;
	};

}
