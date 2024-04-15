# The input JSON file
$input_file = "utilities/version/version.json"

# Parse the JSON file to extract the constants
$json = Get-Content $input_file | ConvertFrom-Json
$major = $json.PRETTY_SER_VERSION_MAJOR
$minor = $json.PRETTY_SER_VERSION_MINOR
$rev = $json.PRETTY_SER_VERSION_REV

# Echo the version string
echo "$major.$minor.$rev"
