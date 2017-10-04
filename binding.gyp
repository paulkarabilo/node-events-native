{
  'targets': [
    {
      'target_name': 'native_events',
      'sources': ['./src/native-events.cc'],
      'include_dirs' : ["<!(node -e \"require('nan')\")", "."]
    }
  ]
}