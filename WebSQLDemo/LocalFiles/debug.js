/**
 * Prints all fields in an object to inspect it.
 */
function printObject(obj, indent)
{
	if (!indent)
	{
		indent = "";
	}

	console.log(indent + "@@@ " + obj);

	indent += "  ";

	for (var field in obj)
	{
		try
		{
			if (typeof obj[field] != "function")
			{
				console.log(indent + field + ": " + obj[field]);
				if ((null != obj[field]) && (typeof obj[field] == "object"))
				{
					printObject(obj[field], indent);
				}
			}
		}
		catch (ex)
		{
			//console.log(indent + "Exception: " + ex)
		}
	}
}
