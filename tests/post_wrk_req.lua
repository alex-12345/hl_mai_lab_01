counter = 0

wrk.method = "POST"
wrk.headers["Content-Type"] = "application/x-www-form-urlencoded"

request = function()
   path = "/person"
   wrk.body = "last_name=Vinnilov&first_name=Alexey&age=22&login=alex-" .. counter
   counter = counter + 1
   return wrk.format(nil, path)
end
