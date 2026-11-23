require 'yaml'
require 'open3'

root = File.expand_path('..', __dir__)
workflow = YAML.load_file(File.join(root, '.github/workflows/ue5-build.yml'))
triggers = workflow['on'] || workflow[true]
raise 'Missing workflow triggers' unless triggers.is_a?(Hash)
raise 'Push trigger must target main' unless triggers.dig('push', 'branches') == ['main']
raise 'Pull request trigger must target main' unless triggers.dig('pull_request', 'branches') == ['main']
raise 'Manual workflow_dispatch input is missing' unless triggers['workflow_dispatch'].is_a?(Hash)

input = triggers.dig('workflow_dispatch', 'inputs', 'run_native_ue_build')
raise 'Native UE build must default to false' unless input && input['default'] == false
native = workflow.dig('jobs', 'native-ue-build')
raise 'Native UE job is missing' unless native.is_a?(Hash)
condition = native['if'].to_s
raise 'Native UE job must be dispatch-only and opt-in' unless condition.include?('workflow_dispatch') && condition.include?('run_native_ue_build')

checked = 0
walk = lambda do |node|
  case node
  when Hash
    if node['run'].is_a?(String)
      out, err, status = Open3.capture3('bash', '-n', stdin_data: node['run'])
      raise "Bash syntax check failed: #{err}#{out}" unless status.success?
      checked += 1
    end
    node.each_value { |value| walk.call(value) }
  when Array
    node.each { |value| walk.call(value) }
  end
end
walk.call(workflow)
raise 'No workflow shell blocks were checked' if checked == 0
puts "YAML valid; main push/PR triggers and opt-in dispatch verified; #{checked} Bash run blocks parse."
